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

typedef struct ScopedMockFunction {
  std::string name;
  std::vector<std::string> full_scope;
  ScopeInfo::Kind kind;
  std::vector<std::string> mock_function_declaration;
  std::shared_ptr<FunctionAttributeInterface> p_if;
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

void AddSingletonDefinition(const std::string &class_name, cppcodegen::Class *p_class) {
  // singleton settings for stub
  std::string mock_singleton_signature = class_name + "& GetInstance()";
  cppcodegen::Block singleton_getinstance(cppcodegen::definition_t, mock_singleton_signature);
  singleton_getinstance << "static " + class_name + " instance;"
                        << "return &instance;";
  *p_class << cppcodegen::AccessSpecifier::kPublic << "~" + class_name + "() = default;" << singleton_getinstance;
  *p_class << cppcodegen::AccessSpecifier::kPrivate << class_name + "() = default;";
}

bool AddMockFunction(std::vector<ScopedMockFunction> *p_map, const std::vector<std::string> &scope,
                     const std::string &function, std::shared_ptr<FunctionAttributeInterface> p_if) {
  auto it = std::find_if(p_map->begin(), p_map->end(),
                         [&scope](const ScopedMockFunction &info) { return info.full_scope == scope; });
  if (it != p_map->end()) {
    it->mock_function_declaration.push_back(function);
    it->p_if = p_if;
    return true;
  }
  for (auto &&it_each : *p_map) {
    if (AddMockFunction(&it_each.children, scope, function, p_if)) {
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
        // singleton settings for stub
        AddSingletonDefinition(mock_class_name + kMockFreeFunctionClassName, &free_function_class);
        body_all << (free_function_class << cppcodegen::AccessSpecifier::kPublic << body);
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
        } else {
          // singleton settings for stub
          AddSingletonDefinition(mock_class_name + it.name, &s_class);
        }
        body_all << (s_class << cppcodegen::AccessSpecifier::kPublic << body);  // public only
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
  cppcodegen::Snippet include_snippets;
  std::vector<std::string> headers;
  for (const auto &it : includes) {
    std::string out;
    if (it.kind == IncludeInfo::Kind::kSystem) {
      cppcodegen::Snippet system_include(cppcodegen::system_include_t);
      out = (system_include << it.name).Out();
    } else if (it.kind == IncludeInfo::Kind::kLocal) {
      cppcodegen::Snippet local_include(cppcodegen::local_include_t, "");
      out = (local_include << it.name).Out();
    }
    headers.emplace_back(out.substr(0, out.size() - 1));  // without \n
  }
  std::sort(headers.begin(), headers.end());
  headers.erase(std::unique(headers.begin(), headers.end()), headers.end());
  include_snippets.Add(headers);
  return include_snippets.Out();
}

// std::string GenerateStubFunction(const std::vector<ScopedMockFunction> &map,
//                                  const std::unordered_map<std::string, std::vector<std::string>> &class_bases_map,
//                                  const std::string &mock_class_name) {
//   std::string stub_definition;
//   // free function
//   for (const auto &it : map) {
//     if (it.kind == ScopeInfo::Kind::kGlobal) {
//       cppcodegen::Block function(cppcodegen::definition_t, it.p_if->Declaration());
//     }
//   }
//   return stub_definition;
// }

}  // namespace

bool GoogleMockHarness::Ready() noexcept {
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
      AddMockFunction(&map, it->Scope().scope_names, declaration, it);
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
  cppcodegen::Snippet file;
  file << "// " + notice_message_;

  file << std::string(kMockFileHeader) << std::string(kMockIncludeHeader);
  file << AddIncludes(p_analyzer_->GetIncludes()) << "\n";
  file << GenerateMockBody(map, class_bases_map, name_);

  body_ = file.Out();

  return true;
}
