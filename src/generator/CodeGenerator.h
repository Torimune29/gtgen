#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ScopeInfo.h"

class CodeGenerator {
 public:
  virtual ~CodeGenerator() = default;
  virtual void StartScope(const std::string &name) const noexcept = 0;
  virtual void EndScope() const noexcept = 0;

 protected:
  CodeGenerator() = default;

  size_t indent_;
};

class GoogleMockLegacyDecorator : public FunctionAttributeDecorator {
 public:
  GoogleMockLegacyDecorator(std::shared_ptr<FunctionAttributeInterface> p_if, bool public_only = true);
  ~GoogleMockLegacyDecorator() override = default;

  std::string Declaration() const noexcept final;

 private:
  bool public_only_;
};

class GoogleMockDecorator : public FunctionAttributeDecorator {
 public:
  explicit GoogleMockDecorator(std::shared_ptr<FunctionAttributeInterface> p_if);
  ~GoogleMockDecorator() override;

  std::string Declaration() const noexcept final;

 private:
  bool public_only_;
};
