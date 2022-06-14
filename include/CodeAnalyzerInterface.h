#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "FunctionInfo.h"
#include "IncludeInfo.h"
#include "ScopeInfo.h"

/**
 * @brief Abstract Code Parser Class
 *
 */
class CodeAnalyzerInterface {
 public:
  virtual ~CodeAnalyzerInterface() = default;

  virtual std::vector<std::shared_ptr<FunctionAttributeInterface>> GetFunctions() {
    return {};
  }

  virtual std::vector<ScopeInfo> GetScopes() {
    return {};
  }

  virtual std::vector<IncludeInfo> GetIncludes() {
    return {};
  }

 protected:
  CodeAnalyzerInterface() = default;
};
