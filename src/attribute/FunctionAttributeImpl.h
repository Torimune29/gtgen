#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "FunctionAttribute.h"
#include "FunctionInfo.h"

class FunctionAttributeBase : public FunctionAttributeInterface {
 public:
  virtual ~FunctionAttributeBase() override;

  bool operator==(const FunctionAttributeInterface &rhs) const noexcept final;
  bool operator!=(const FunctionAttributeInterface &rhs) const noexcept final;

  bool InScopeOf(const FunctionScope &rhs) const noexcept final;

  std::string ConstantExpression() const noexcept final;  // 5.19 Constant expressions
  std::string DefinitionSuffix() const noexcept final;    // 8.4.3 Deleted definitions
  std::string ExceptionSuffix() const noexcept final;     // 15.4 Exception specification
  FunctionScope Scope() const noexcept final;
  std::string Name() const noexcept final;
  std::string ReturnType() const noexcept final;
  std::string Signature() const noexcept final;
  std::vector<std::string> ParameterTypes() const noexcept final;
  std::vector<std::pair<std::string, std::string>> Parameters() const noexcept final;
  bool IsOverloadedOperator() const noexcept final;
  std::string Declaration() const noexcept final;

 protected:
  explicit FunctionAttributeBase(const FunctionInfoBase &info);

 private:
  FunctionInfoBase info_;
};

class MemberFunctionAttribute : public FunctionAttributeBase {
 public:
  explicit MemberFunctionAttribute(const MemberFunctionInfo &info);
  virtual ~MemberFunctionAttribute() override;

  bool IsClassMember() const noexcept final;           // 9.2 Class members
  std::string AccessSpecifier() const noexcept final;  // 9.2 Class members
  std::string CvQualifier() const noexcept final;      // 9.3 cv-qualifiers
  bool IsAbleToPolymorphic() const noexcept final;
  std::vector<std::string> BaseClasses() const noexcept final;

  std::string StorageClass() const noexcept final;  // At most one (7.1.1 Storage class specifiers)

  MemberFunctionInfo GetAttribute();

 private:
  MemberFunctionInfo info_;
};

class NamespaceFunctionAttribute : public FunctionAttributeBase {
 public:
  explicit NamespaceFunctionAttribute(const FunctionInfo &info);
  virtual ~NamespaceFunctionAttribute() override;

  bool IsClassMember() const noexcept final;           // 9.2 Class members
  std::string AccessSpecifier() const noexcept final;  // 9.2 Class members
  std::string CvQualifier() const noexcept final;      // 9.3 cv-qualifiers
  bool IsAbleToPolymorphic() const noexcept final;
  std::vector<std::string> BaseClasses() const noexcept final;

  std::string StorageClass() const noexcept final;  // At most one (7.1.1 Storage class specifiers)

  FunctionInfo GetAttribute();

 private:
  FunctionInfo info_;
};
