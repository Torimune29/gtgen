#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "AbstractCodeParser.h"

/**
 * @brief Abstract Code Parser Class
 *
 */
template<typename T>
class AbstractTestHarness {
 public:
  virtual ~AbstractTestHarness() = default;
  virtual bool Ready() = 0;

  std::string Create() const noexcept {
    return body_;
  }

 protected:
  AbstractTestHarness(const std::string &name, std::shared_ptr<T> p_parser)
    : name_(name) , p_parser_(p_parser) {}

  std::string name_;
  std::shared_ptr<T> p_parser_;
  std::string body_;
};
