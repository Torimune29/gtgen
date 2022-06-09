#include "GoogleMockHarness.h"

namespace {
const char kMockMethodName[] = "MOCK_METHOD";

}  // namespace


bool GoogleMockHarness::Ready() noexcept {
  bool okay = p_parser_->Ready();
  if (okay) {
    auto v = p_parser_->GetFunction();
    for (const auto &it : v) {
      std::string mock_method(kMockMethodName);
      mock_method += '0';
      mock_method += '(';
      mock_method += it.base.name;
      mock_method += ", ";
      mock_method += it.base.return_type;
      mock_method += it.base.signature;
      mock_method += ")\n";
      body_ += mock_method;
    }
  }
  return okay;
}
