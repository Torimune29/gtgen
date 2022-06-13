#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "AbstractTestHarness.h"
#include "CodeAnalyzerInterface.h"

/**
 * @brief  Code Parser Class
 *
 */
class TestTargetFunctionViewerHarness : public AbstractTestHarness {
 public:
  TestTargetFunctionViewerHarness(std::shared_ptr<CodeAnalyzerInterface> p_analyzer)
    : AbstractTestHarness("")
    , p_analyzer_(p_analyzer) {}
  ~TestTargetFunctionViewerHarness() override = default;
  bool Ready() noexcept final;
 private:
  std::shared_ptr<CodeAnalyzerInterface> p_analyzer_;

};

/**
 * @brief  Code Parser Class
 *
 */
class TestTargetScopeRelationViewerHarness : public AbstractTestHarness {
 public:
  TestTargetScopeRelationViewerHarness(std::shared_ptr<CodeAnalyzerInterface> p_analyzer)
    : AbstractTestHarness("")
    , p_analyzer_(p_analyzer) {}
  ~TestTargetScopeRelationViewerHarness() override = default;
  bool Ready() noexcept final;
 private:
  std::shared_ptr<CodeAnalyzerInterface> p_analyzer_;

};

/**
 * @brief  Code Parser Class
 *
 */
class TestTargetLocalIncludeViewerHarness : public AbstractTestHarness {
 public:
  TestTargetLocalIncludeViewerHarness(std::shared_ptr<CodeAnalyzerInterface> p_analyzer)
    : AbstractTestHarness("")
    , p_analyzer_(p_analyzer) {}
  ~TestTargetLocalIncludeViewerHarness() override = default;
  bool Ready() noexcept final;
 private:
  std::shared_ptr<CodeAnalyzerInterface> p_analyzer_;

};