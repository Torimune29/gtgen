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
class TestTargetViewerHarness : public AbstractTestHarness<FunctionParser> {
 public:
  TestTargetViewerHarness(std::shared_ptr<FunctionParser> p_parser)
    : AbstractTestHarness("", p_parser) {}
  ~TestTargetViewerHarness() override = default;
  bool Ready() noexcept final;

};
