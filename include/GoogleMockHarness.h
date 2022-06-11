#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "AbstractTestHarness.h"
#include "FunctionParser.h"

/**
 * @brief  Code Parser Class
 *
 */
// class GoogleTestHarness : public AbstractTestHarness<FunctionParser> {
//  public:
//   GoogleTestHarness(const std::string &name, std::shared_ptr<FunctionParser> p_parser)
//     : AbstractTestHarness(name, p_parser) {}
//   ~GoogleTestHarness() override = default;
//   bool Ready() noexcept final;
//
// };

/**
 * @brief  Code Parser Class
 *
 */
class GoogleMockHarness : public AbstractTestHarness {
 public:
  GoogleMockHarness(const std::string &name, std::shared_ptr<FunctionParser> p_parser, bool public_only = true)
    : AbstractTestHarness(name)
    , public_only_(public_only)
    , p_parser_(p_parser) {}
  ~GoogleMockHarness() override = default;
  bool Ready() noexcept final;
 private:
  bool public_only_;
  std::shared_ptr<FunctionParser> p_parser_;

};
