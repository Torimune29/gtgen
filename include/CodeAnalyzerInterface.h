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


  virtual std::vector<std::shared_ptr<FunctionAttributeInterface>> GetFunctions() noexcept {
    return {};
  }


  virtual std::vector<ScopeInfo> GetScopes() noexcept {
    return {};
  }

 protected:
  CodeAnalyzerInterface() = default;
};

