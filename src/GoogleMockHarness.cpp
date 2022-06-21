#include "GoogleMockHarness.h"

#include <algorithm>
#include <iostream>

#include "CodeAnalyzerInterface.h"
#include "FunctionAttributeDecorator.h"
#include "cppcodegen.h"

namespace {
const char kMockFileHeader[] = "#pragma once";
const char kMockIncludeHeader[] = "#include <gmock/gmock.h>";
const char kMockFreeFunctionClassName[] = "FreeFunction";
const char kMockFreeFunctionScopeName[] = "@FreeFunction@";

const size_t kIncludeStringMaxSize = 255;
const char kSystemIncludeFormat[] = R"xxx(#include <%s>)xxx";
const char kLocalIncludeFormat[] = R"xxx(#include "%s")xxx";

typedef struct ScopedMockFunction {
  std::string name;
  std::vector<std::string> full_scope;
  ScopeInfo::Kind kind;
  std::vector<std::string> mock_function_declaration;
  std::vector<ScopedMockFunction> children;
} ScopedMockFunction;

std::vector<ScopedMockFunction> InitializeScopedFunction(const std::vector<ScopeInfo> &infos, bool expect_global) {
  std::vector<ScopedMockFunction> functions;
  // for free function
  if (expect_global) {
    ScopedMockFunction free_function;
    free_function.name = kMockFreeFunctionScopeName;
    free_function.kind = ScopeInfo::Kind::kGlobal;
    free_function.full_scope = {""};
    functions.push_back(free_function);
  }

  // scope
  for (const auto &it : infos) {
    ScopedMockFunction function;
    function.name = it.name;
    function.full_scope = it.full_scope;
    function.kind = it.kind;
    function.children = InitializeScopedFunction(it.children, false);
    functions.push_back(function);
  }
  return functions;
}

bool AddMockFunction(std::vector<ScopedMockFunction> *p_map, const std::vector<std::string> &scope,
                     const std::string &function) {
  auto it = std::find_if(p_map->begin(), p_map->end(),
                         [&scope](const ScopedMockFunction &info) { return info.full_scope == scope; });
  if (it != p_map->end()) {
    it->mock_function_declaration.push_back(function);
    return true;
  }
  for (auto &&it_each : *p_map) {
    if (AddMockFunction(&it_each.children, scope, function)) {
      return true;
    }
  }
  return false;
}

std::string GenerateMockBody(const std::vector<ScopedMockFunction> &map,
                             const std::unordered_map<std::string, std::vector<std::string>> &class_bases_map,
                             const std::string &mock_class_name) {
  cppcodegen::Snippet body_all;
  for (const auto &it : map) {
    // generate child function
    auto body_child = GenerateMockBody(it.children, class_bases_map, mock_class_name);
    if (body_child.empty() && it.mock_function_declaration.empty()) {  // no children and descendant
      continue;
    }

    cppcodegen::Snippet body;
    // add now scope function
    for (const auto &it_function : it.mock_function_declaration) {
      // only support free or class member function
      if (it.kind == ScopeInfo::Kind::kGlobal || it.kind == ScopeInfo::Kind::kClass) {
        body << it_function;
      }
    }
    // add child scope
    body << body_child;

    // concat scope
    switch (it.kind) {
      case ScopeInfo::Kind::kGlobal: {
        cppcodegen::Class free_function_class(mock_class_name + kMockFreeFunctionClassName);
        body_all << (free_function_class << body);
        break;
      }
      case ScopeInfo::Kind::kNamespace: {
        cppcodegen::Block namespace_block(cppcodegen::namespace_t, it.name);
        body_all << (namespace_block << body);
        break;
      }
      case ScopeInfo::Kind::kClass: {
        cppcodegen::Class s_class(mock_class_name + it.name);
        if (class_bases_map.count(it.name) != 0 && !class_bases_map.at(it.name).empty()) {
          for (const auto &it_base_class : class_bases_map.at(it.name)) {
            s_class.AddInheritance(it_base_class, cppcodegen::AccessSpecifier::kPublic);
          }
        }
        body_all << (s_class << body);
        break;
      }
      default: {
        break;
      }
    }
  }
  return body_all.Out();
}

std::string AddIncludes(const std::vector<IncludeInfo> &includes) {
  std::vector<std::string> headers;
  char header_buf[kIncludeStringMaxSize] = {};
  for (const auto &it : includes) {
    int header_buf_size = 0;
    if (it.kind == IncludeInfo::Kind::kSystem) {
      header_buf_size = snprintf(header_buf, kIncludeStringMaxSize, kSystemIncludeFormat, it.name.c_str());
    } else if (it.kind == IncludeInfo::Kind::kLocal) {
      header_buf_size = snprintf(header_buf, kIncludeStringMaxSize, kLocalIncludeFormat, it.name.c_str());
    }
    if (header_buf_size > 0) headers.push_back(std::string(header_buf, static_cast<size_t>(header_buf_size)));
  }
  std::sort(headers.begin(), headers.end());
  headers.erase(std::unique(headers.begin(), headers.end()), headers.end());

  std::string header_lines;
  for (const auto &it : headers) {
    header_lines += it + "\n";
  }
  return header_lines;
}

}  // namespace

bool GoogleMockHarness::Ready() noexcept {
  body_ += "// " + notice_message_ + '\n';

  std::unordered_map<std::string, std::vector<std::string>> class_bases_map;

  // scope
  auto v_scope = p_analyzer_->GetScopes();
  auto map = InitializeScopedFunction(v_scope, true);

  // function
  auto v_func = p_analyzer_->GetFunctions();
  for (const auto &it : v_func) {
    GoogleMockLegacyDecorator decorator(it, public_only_);
    auto declaration = decorator.Declaration();
    if (!declaration.empty()) {
      AddMockFunction(&map, it->Scope().scope_names, declaration);
      if (it->IsAbleToPolymorphic() && !it->Scope().scope_names.empty()) {
        auto base_classes = it->BaseClasses();
        if (base_classes.empty()) {
          // target class is base
          class_bases_map[it->Scope().scope_names.back()] = std::vector<std::string>({it->Scope().scope_names.back()});
        } else {
          class_bases_map[it->Scope().scope_names.back()] = it->BaseClasses();
        }
      }
    }
  }

  // generate mock
  body_ += std::string(kMockFileHeader) + "\n\n" + std::string(kMockIncludeHeader) + "\n\n";
  body_ += AddIncludes(p_analyzer_->GetIncludes()) + "\n\n";
  body_ += GenerateMockBody(map, class_bases_map, name_);

  return true;
}
