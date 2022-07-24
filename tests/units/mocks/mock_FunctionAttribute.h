// This code was generated by gtgen 0.0.1
#pragma once
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class mock_FunctionAttributeInterface : public FunctionAttributeInterface {
 public:
  MOCK_CONST_METHOD1(operator_12312198772084833146, bool(FunctionAttributeInterface const&));
  bool operator== (FunctionAttributeInterface const& rhs) const noexcept {
    return this->operator_12312198772084833146(rhs);
  }
  
  MOCK_CONST_METHOD1(operator_12779008329652289595, bool(FunctionAttributeInterface const&));
  bool operator!= (FunctionAttributeInterface const& rhs) const noexcept {
    return this->operator_12779008329652289595(rhs);
  }
  
  MOCK_CONST_METHOD1(InScopeOf_noexcept, bool(FunctionScope const&));
  bool InScopeOf (FunctionScope const& rhs) const noexcept {
    return this->InScopeOf_noexcept(rhs);
  }
  
  MOCK_CONST_METHOD0(ConstantExpression_noexcept, std::string());
  std::string ConstantExpression () const noexcept {
    return this->ConstantExpression_noexcept();
  }
  
  MOCK_CONST_METHOD0(DefinitionSuffix_noexcept, std::string());
  std::string DefinitionSuffix () const noexcept {
    return this->DefinitionSuffix_noexcept();
  }
  
  MOCK_CONST_METHOD0(ExceptionSuffix_noexcept, std::string());
  std::string ExceptionSuffix () const noexcept {
    return this->ExceptionSuffix_noexcept();
  }
  
  MOCK_CONST_METHOD0(Scope_noexcept, FunctionScope());
  FunctionScope Scope () const noexcept {
    return this->Scope_noexcept();
  }
  
  MOCK_CONST_METHOD0(Name_noexcept, std::string());
  std::string Name () const noexcept {
    return this->Name_noexcept();
  }
  
  MOCK_CONST_METHOD0(ReturnType_noexcept, std::string());
  std::string ReturnType () const noexcept {
    return this->ReturnType_noexcept();
  }
  
  MOCK_CONST_METHOD0(Signature_noexcept, std::string());
  std::string Signature () const noexcept {
    return this->Signature_noexcept();
  }
  
  MOCK_CONST_METHOD0(ParameterTypes_noexcept, std::vector<std::string>());
  std::vector<std::string> ParameterTypes () const noexcept {
    return this->ParameterTypes_noexcept();
  }
  
  MOCK_CONST_METHOD0(Parameters_noexcept, std::vector<std::pair<std::string, std::string>>());
  std::vector<std::pair<std::string, std::string>> Parameters () const noexcept {
    return this->Parameters_noexcept();
  }
  
  MOCK_CONST_METHOD0(IsOverloadedOperator_noexcept, bool());
  bool IsOverloadedOperator () const noexcept {
    return this->IsOverloadedOperator_noexcept();
  }
  
  MOCK_CONST_METHOD0(Declaration_noexcept, std::string());
  std::string Declaration () const noexcept {
    return this->Declaration_noexcept();
  }
  
  MOCK_CONST_METHOD0(IsClassMember_noexcept, bool());
  bool IsClassMember () const noexcept {
    return this->IsClassMember_noexcept();
  }
  
  MOCK_CONST_METHOD0(AccessSpecifier_noexcept, std::string());
  std::string AccessSpecifier () const noexcept {
    return this->AccessSpecifier_noexcept();
  }
  
  MOCK_CONST_METHOD0(CvQualifier_noexcept, std::string());
  std::string CvQualifier () const noexcept {
    return this->CvQualifier_noexcept();
  }
  
  MOCK_CONST_METHOD0(IsAbleToPolymorphic_noexcept, bool());
  bool IsAbleToPolymorphic () const noexcept {
    return this->IsAbleToPolymorphic_noexcept();
  }
  
  MOCK_CONST_METHOD0(BaseClasses_noexcept, std::vector<std::string>());
  std::vector<std::string> BaseClasses () const noexcept {
    return this->BaseClasses_noexcept();
  }
  
  MOCK_CONST_METHOD0(StorageClass_noexcept, std::string());
  std::string StorageClass () const noexcept {
    return this->StorageClass_noexcept();
  }
  
  
};

