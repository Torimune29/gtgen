#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "FunctionInfo.h"
#include "ScopeInfo.h"
#include "IncludeInfo.h"

/**
 * @brief Abstract Code Parser Class
 *
 */
class CodeAnalyzerResultStorage : public CodeAnalyzerInterface {
 public:
  CodeAnalyzerResultStorage() = default;
  ~CodeAnalyzerResultStorage() override = default;

  std::vector<std::shared_ptr<FunctionAttributeInterface>> GetFunctions() noexcept override {
    std::vector<std::shared_ptr<FunctionAttributeInterface>> v;
    for (const auto &it : analyzers_) {
      auto v_result = it->GetFunctions();
      v.insert(v.end(), v_result.begin(), v_result.end());
    }
    return v;
  }
  std::vector<ScopeInfo> GetScopes() noexcept override {
    std::vector<ScopeInfo> v;
    for (const auto &it : analyzers_) {
      auto v_result = it->GetScopes();
      v.insert(v.end(), v_result.begin(), v_result.end());
    }
    return v;
  }

  std::vector<IncludeInfo> GetIncludes() noexcept override {
    std::vector<IncludeInfo> v;
    for (const auto &it : analyzers_) {
      auto v_result = it->GetIncludes();
      v.insert(v.end(), v_result.begin(), v_result.end());
    }
    return v;
  }

  void AddAnalyzer(std::shared_ptr<CodeAnalyzerInterface> p_if) {
    analyzers_.emplace_back(p_if);
  }

 private:
  std::vector<std::shared_ptr<CodeAnalyzerInterface>> analyzers_;
};
