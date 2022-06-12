#pragma once

#include <string>
#include <memory>
#include "ProjectInformation.h"

/**
 * @brief Abstract Code Parser Class
 *
 */
class AbstractTestHarness {
 public:
  virtual ~AbstractTestHarness();
  virtual bool Ready() = 0;

  std::string Generate() const noexcept;

 protected:
  explicit AbstractTestHarness(const std::string &name);
  std::string name_;
  std::string notice_message_;
  std::string body_;
};
