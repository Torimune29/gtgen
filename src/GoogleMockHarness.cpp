#include "GoogleMockHarness.h"
#include <algorithm>
#include <iostream>

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

std::string GenerateMockMethod(const char header[], const FunctionBase &base) {
   std::string mock_method(header);
   mock_method += std::to_string(base.parameters.size());
   mock_method += "(" + base.name + ", " + base.return_type + base.signature + ");\n";
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

bool AddMockFunction(std::vector<ScopedMockFunction> *p_map, const std::vector<std::string> &scope, const std::string &function) {
  auto it = std::find_if(p_map->begin(), p_map->end(), [&scope] (const ScopedMockFunction &info) {
    return info.full_scope == scope;
  });
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
  bool okay_function = p_function_parser_->Ready();
  bool okay_scope = p_scope_relation_parser_->Ready();
  if (!(okay_function && okay_scope) ) return false;

  body_ += "// " + notice_message_ + '\n';

  std::unordered_map<std::string, std::vector<std::string>> class_function_map;
  std::unordered_map<std::string, std::vector<std::string>> class_bases_map;

  // scope
  auto v_scope = p_scope_relation_parser_->Get();
  auto map = InitializeScopedFunction(v_scope, true);

  // function
  auto v_func = p_function_parser_->GetFunction();
  for (const auto &it : v_func) {
    if (it.base.is_deleted)
      continue;
    AddMockFunction(&map, it.base.scope, GenerateMockMethod(kMockMethodName, it.base));
  }

  // class
  auto v_member_func = p_function_parser_->GetMemberFunction();
  for (const auto &it : v_member_func) {
    if (it.base.is_deleted
      || (public_only_ && it.access_specifier != MemberFunctionInfo::AccessSpecifier::kPublic)
      || it.is_volatile) {  // not support volatile method mocking
      continue;
    }
    if (it.class_name.empty()) {
      AddMockFunction(&map, it.base.scope, GenerateMockMethod(kMockMethodName, it.base));
    } else {
      if (it.is_const) {
        AddMockFunction(&map, it.base.scope, GenerateMockMethod(kMockConstMethodName, it.base));
      } else {
        AddMockFunction(&map, it.base.scope, GenerateMockMethod(kMockMethodName, it.base));
      }
      if (class_bases_map[it.class_name].empty())
        class_bases_map[it.class_name] = it.base_classes;
    }
  }


  // derrived check
  for(const auto &it : class_function_map) {
    auto class_name = it.first;
    bool mock_is_able_to_derrived = true;
    std::for_each(v_member_func.begin(), v_member_func.end(), [class_name, &mock_is_able_to_derrived] (const MemberFunctionInfo &info) {
      if (info.class_name == class_name && !info.is_polymorphic)
        mock_is_able_to_derrived = false;
    });
    if (mock_is_able_to_derrived) {
      if (class_bases_map[class_name].empty()) {  // Target class is a base class
        class_bases_map[class_name].push_back(class_name);
      }
    } else {
      class_bases_map[class_name].clear();
    }
  }

  // generate mock
  body_ += std::string(kMockFileHeader) + "\n\n"
        + std::string(kMockIncludeHeader) + "\n\n";
  body_ += GenerateMockBody(map, class_bases_map, name_);

  // for (const auto &it : class_function_map) {
  //   body_ += "class " + std::string(kMockClassNamePrefix) + it.first;
  //   if (class_bases_map.count(it.first) != 0
  //     && !class_bases_map[it.first].empty()) {
  //     body_ += " : public ";
  //     for (const auto &it_class : class_bases_map[it.first]) {
  //       body_ += it_class + ", ";
  //     }
  //     body_ = body_.substr(0, body_.size() - 2);
  //   }
  //   body_ += " {\n";
  //   for (const auto &it_method : it.second) {
  //     body_ += "  " + it_method;
  //   }
  //   body_ += "};\n";
  // }

  return true;
}

