#include "GoogleMockHarness.h"

namespace {
const char kMockMethodName[] = "MOCK_METHOD";
const char kMockConstMethodName[] = "MOCK_CONST_METHOD";

std::string GenerateMockMethod(const char header[], const FunctionBase &base) {
   std::string mock_method(header);
   mock_method += std::to_string(base.parameters.size());
   mock_method += "(" + base.name + ", " + base.return_type + base.signature + ");\n";
   return mock_method;
}

}  // namespace

bool GoogleMockHarness::Ready() noexcept {
  bool okay = p_parser_->Ready();
  if (okay) {
    auto v_func = p_parser_->GetFunction();
    for (const auto &it : v_func) {
      if (it.base.is_deleted)
        continue;
      body_ += GenerateMockMethod(kMockMethodName, it.base);
    }
    auto v_member_func = p_parser_->GetMemberFunction();
    for (const auto &it : v_member_func) {
      if (it.base.is_deleted
        || (public_only_ && it.access_specifier != MemberFunctionInfo::AccessSpecifier::kPublic)) {
        continue;
      }
      if (it.is_const) {
        body_ += GenerateMockMethod(kMockConstMethodName, it.base);
      } else {
        body_ += GenerateMockMethod(kMockMethodName, it.base);
      }
    }
  }
  return okay;
}
