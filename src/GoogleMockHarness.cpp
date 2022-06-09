#include "GoogleMockHarness.h"

namespace {
const char kMockMethodName[] = "MOCK_METHOD";
const char kMockConstMethodName[] = "MOCK_CONST_METHOD";

}  // namespace

bool GoogleMockHarness::Ready() noexcept {
  bool okay = p_parser_->Ready();
  if (okay) {
    auto v_func = p_parser_->GetFunction();
    for (const auto &it : v_func) {
      std::string mock_method(kMockMethodName);
      mock_method += std::to_string(it.base.parameters.size());
      mock_method += '(';
      mock_method += it.base.name;
      mock_method += ", ";
      mock_method += it.base.return_type;
      mock_method += it.base.signature;
      mock_method += ");\n";
      body_ += mock_method;
    }
    auto v_member_func = p_parser_->GetMemberFunction();
    for (const auto &it : v_member_func) {
      if (it.access_specifier != MemberFunctionInfo::AccessSpecifier::kPublic) continue;
      std::string mock_method;
      if (it.is_const) {
        mock_method = kMockConstMethodName;
      } else {
        mock_method = kMockMethodName;
      }
      mock_method += std::to_string(it.base.parameters.size());
      mock_method += '(';
      mock_method += it.base.name;
      mock_method += ", ";
      mock_method += it.base.return_type;
      mock_method += it.base.signature;
      mock_method += ");\n";
      body_ += mock_method;
    }
  }
  return okay;
}
