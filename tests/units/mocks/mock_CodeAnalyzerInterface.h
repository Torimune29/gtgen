// This code was generated by gtgen 0.0.1
#pragma once

#include <gmock/gmock.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "FunctionAttribute.h"
#include "IncludeInfo.h"
#include "ScopeInfo.h"

class mock_CodeAnalyzerInterface : public CodeAnalyzerInterface {
 public:
  MOCK_METHOD0(GetFunctions_noexcept, std::vector<std::shared_ptr<FunctionAttributeInterface>>());
  std::vector<std::shared_ptr<FunctionAttributeInterface>> GetFunctions() noexcept {
    return this->GetFunctions_noexcept();
  }

  MOCK_METHOD0(GetScopes_noexcept, std::vector<ScopeInfo>());
  std::vector<ScopeInfo> GetScopes() noexcept {
    return this->GetScopes_noexcept();
  }

  MOCK_METHOD0(GetIncludes_noexcept, std::vector<IncludeInfo>());
  std::vector<IncludeInfo> GetIncludes() noexcept {
    return this->GetIncludes_noexcept();
  }
};
