#include "GoogleMockHarness.h"

#include <algorithm>
#include <iostream>

#include "CodeAnalyzerInterface.h"

namespace {
const char kMockFileHeader[] = "#pragma once";
const char kMockIncludeHeader[] = "#include <gmock/gmock.h>";
const char kMockMethodName[] = "MOCK_METHOD";
const char kMockConstMethodName[] = "MOCK_CONST_METHOD";
const char kMockFreeFunctionClassName[] = "FreeFunction";
const char kMockFreeFunctionScopeName[] = "@FreeFunction@";

typedef struct ScopedMockFunction {
  std::string name;
  std::vector<std::string> full_scope;
  ScopeInfo::Kind kind;
  std::vector<std::string> mock_function_declaration;
  std::vector<ScopedMockFunction> children;
} ScopedMockFunction;

std::string GenerateMockMethod(const char header[], const FunctionAttributeInterface *const interface) {
  std::string mock_method(header);
  mock_method += std::to_string(interface->Parameters().size());
  mock_method += "(" + interface->Name() + ", " + interface->ReturnType() + interface->ParameterList() + ");\n";
  return mock_method;
}

std::vector<ScopedMockFunction> InitializeScopedFunction(const std::vector<ScopeInfo> &infos, bool expect_global) {
  std::vector<ScopedMockFunction> functions;
  // for free function
  if (expect_global) {
    ScopedMockFunction free_function;
    free_function.name = kMockFreeFunctionScopeName;
    free_function.kind = ScopeInfo::Kind::kGlobal;
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
  std::string body_all;
  for (const auto &it : map) {
    std::string body;
    // generate scope
    switch (it.kind) {
      case ScopeInfo::Kind::kGlobal: {
        body += "class " + mock_class_name + std::string(kMockFreeFunctionClassName) + " {\n";
        break;
      }
      case ScopeInfo::Kind::kNamespace: {
        body += "namespace " + it.name + " {\n";
        break;
      }
      case ScopeInfo::Kind::kClass: {
        body += "class " + mock_class_name + it.name;
        if (class_bases_map.count(it.name) != 0 && !class_bases_map.at(it.name).empty()) {
          body += " : public ";
          std::string base_class_list;
          for (const auto &it_base_class : class_bases_map.at(it.name)) {
            base_class_list += it_base_class + ", ";
          }
          body += base_class_list.substr(0, base_class_list.size() - 2);
        }
        body += " {\n";
        if (!it.mock_function_declaration.empty()) body += " public:\n";
        break;
      }
      default: {
        break;
      }
    }
    // generate child function
    for (const auto &it_function : it.mock_function_declaration) {
      body += "  " + it_function;
    }
    // generate child scope
    auto body_child = GenerateMockBody(it.children, class_bases_map, mock_class_name);
    if (body_child.empty() && it.mock_function_declaration.empty()) {  // no children and descendant
      body.clear();
    } else {
      body += body_child;
      body += "};\n";
    }
    body_all += body;
  }
  return body_all;
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
    if (it->DefinitionSuffix() == "deleted" ||
        (public_only_ && it->IsClassMember() && it->AccessSpecifier() != "public") ||
        it->CvQualifier().find("volatile") != std::string::npos) {  // not support volatile method mocking
      continue;
    }
    if (it->CvQualifier().find("const") != std::string::npos) {
      AddMockFunction(&map, it->Scope().scope_names, GenerateMockMethod(kMockConstMethodName, it.get()));
    } else {
      AddMockFunction(&map, it->Scope().scope_names, GenerateMockMethod(kMockMethodName, it.get()));
    }
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

  // generate mock
  body_ += std::string(kMockFileHeader) + "\n\n" + std::string(kMockIncludeHeader) + "\n\n";
  body_ += GenerateMockBody(map, class_bases_map, name_);

  return true;
}
