#include "AbstractTestHarness.h"

AbstractTestHarness::AbstractTestHarness(const std::string &name)
    : name_(name), notice_message_("This code was generated by " + std::string(PROJECT_NAME_VERSION)) {
}

AbstractTestHarness::~AbstractTestHarness() = default;

std::string AbstractTestHarness::Generate() const noexcept {
  return body_;
}
