#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "AbstractTestHarness.h"
#include "FunctionParser.h"
#include "ScopeRelationParser.h"

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
  GoogleMockHarness(const std::string &name, std::shared_ptr<FunctionParser> p_function_parser,
                    std::shared_ptr<ScopeRelationParser> p_scope_relation_parser, bool public_only = true)
      : AbstractTestHarness(name),
        public_only_(public_only),
        p_function_parser_(p_function_parser),
        p_scope_relation_parser_(p_scope_relation_parser) {}
  ~GoogleMockHarness() override = default;
  bool Ready() noexcept final;

 private:
  bool public_only_;
  std::shared_ptr<FunctionParser> p_function_parser_;
  std::shared_ptr<ScopeRelationParser> p_scope_relation_parser_;
};
