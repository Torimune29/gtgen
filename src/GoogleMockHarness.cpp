#include "GoogleMockHarness.h"
#include <algorithm>

namespace {
const char kMockFileHeader[] = "#pragma once";
const char kMockIncludeHeader[] = "#include <gmock/gmock.h>";
const char kMockMethodName[] = "MOCK_METHOD";
const char kMockConstMethodName[] = "MOCK_CONST_METHOD";
const char kMockClassNamePrefix[] = "Mock";
const char kMockFreeFunctionClassName[] = "FreeFunction";

std::string GenerateMockMethod(const char header[], const FunctionBase &base) {
   std::string mock_method(header);
   mock_method += std::to_string(base.parameters.size());
   mock_method += "(" + base.name + ", " + base.return_type + base.signature + ");\n";
   return mock_method;
}

}  // namespace

bool GoogleMockHarness::Ready() noexcept {
  bool okay = p_parser_->Ready();
  if (!okay) return okay;

  body_ += "// " + notice_message_ + '\n';

  std::unordered_map<std::string, std::vector<std::string>> class_function_map;
  std::unordered_map<std::string, std::vector<std::string>> class_bases_map;

  // function
  auto v_func = p_parser_->GetFunction();
  for (const auto &it : v_func) {
    if (it.base.is_deleted)
      continue;
    class_function_map[kMockFreeFunctionClassName]
      .push_back(GenerateMockMethod(kMockMethodName, it.base));
  }

  // class
  auto v_member_func = p_parser_->GetMemberFunction();
  for (const auto &it : v_member_func) {
    if (it.base.is_deleted
      || (public_only_ && it.access_specifier != MemberFunctionInfo::AccessSpecifier::kPublic)) {
      continue;
    }
    if (it.class_name.empty()) {
      class_function_map[kMockFreeFunctionClassName]
        .push_back(GenerateMockMethod(kMockMethodName, it.base));
    } else {
      if (it.is_const) {
        class_function_map[it.class_name]
          .push_back(GenerateMockMethod(kMockConstMethodName, it.base));
      } else {
        class_function_map[it.class_name]
          .push_back(GenerateMockMethod(kMockMethodName, it.base));
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
    if (!mock_is_able_to_derrived) {
      class_bases_map[class_name].clear();
    }
  }

  // generate mock
  body_ += std::string(kMockFileHeader) + "\n\n"
        + std::string(kMockIncludeHeader) + "\n\n";
  for (const auto &it : class_function_map) {
    body_ += "class " + std::string(kMockClassNamePrefix) + it.first;
    if (class_bases_map.count(it.first) != 0
      && !class_bases_map[it.first].empty()) {
      body_ += " : public ";
      for (const auto &it_class : class_bases_map[it.first]) {
        body_ += it_class + ", ";
      }
      body_ = body_.substr(0, body_.size() - 2);
    }
    body_ += " {\n";
    for (const auto &it_method : it.second) {
      body_ += "  " + it_method;
    }
    body_ += "};\n";
  }

  return okay;
}
