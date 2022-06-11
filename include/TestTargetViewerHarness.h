#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "AbstractTestHarness.h"
#include "FunctionParser.h"
#include "ScopeRelationParser.h"

/**
 * @brief  Code Parser Class
 *
 */
class TestTargetFunctionViewerHarness : public AbstractTestHarness {
 public:
  TestTargetFunctionViewerHarness(std::shared_ptr<FunctionParser> p_parser)
    : AbstractTestHarness("")
    , p_parser_(p_parser) {}
  ~TestTargetFunctionViewerHarness() override = default;
  bool Ready() noexcept final;
 private:
  std::shared_ptr<FunctionParser> p_parser_;

};

/**
 * @brief  Code Parser Class
 *
 */
class TestTargetScopeRelationViewerHarness : public AbstractTestHarness {
 public:
  TestTargetScopeRelationViewerHarness(std::shared_ptr<ScopeRelationParser> p_parser)
    : AbstractTestHarness("")
    , p_parser_(p_parser) {}
  ~TestTargetScopeRelationViewerHarness() override = default;
  bool Ready() noexcept final;
 private:
  std::shared_ptr<ScopeRelationParser> p_parser_;

};
