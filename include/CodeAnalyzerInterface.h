#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "FunctionInfo.h"
#include "ScopeInfo.h"

/**
 * @brief Abstract Code Parser Class
 *
 */
class CodeAnalyzerInterface {
 public:
  virtual ~CodeAnalyzerInterface() = default;

  virtual std::vector<std::shared_ptr<FunctionAttributeInterface>> GetFunctions() = 0;
  virtual std::vector<ScopeInfo> GetScopes() = 0;

 protected:
  CodeAnalyzerInterface() = default;
};

/**
 * @brief Abstract Code Parser Class
 *
 */
class CodeAnalyzerResultStorage : public CodeAnalyzerInterface {
 public:
  CodeAnalyzerResultStorage() = default;
  ~CodeAnalyzerResultStorage() override = default;

  std::vector<std::shared_ptr<FunctionAttributeInterface>> GetFunctions() override {
    std::vector<std::shared_ptr<FunctionAttributeInterface>> v;
    for (const auto &it : analyzers_) {
      auto v_result = it->GetFunctions();
      v.insert(v.end(), v_result.begin(), v_result.end());
    }
    return v;
  }
  std::vector<ScopeInfo> GetScopes() override {
    std::vector<ScopeInfo> v;
    for (const auto &it : analyzers_) {
      auto v_result = it->GetScopes();
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

