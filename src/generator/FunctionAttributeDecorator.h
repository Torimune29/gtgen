#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "FunctionAttribute.h"

class FunctionAttributeDecorator {
 public:
  virtual ~FunctionAttributeDecorator() = default;

  /**
   * @brief Declaration for each decorator
   *
   * @return std::string
   */
  virtual std::string Declaration() const noexcept = 0;

 protected:
  std::shared_ptr<FunctionAttributeInterface> p_if_;

  explicit FunctionAttributeDecorator(std::shared_ptr<FunctionAttributeInterface> p_if) : p_if_(p_if) {
  }
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
