#include "FunctionInfo.h"
#include <memory>


FunctionAttributeBase::FunctionAttributeBase(const FunctionInfoBase &info)
  : info_(info) {
 }


FunctionAttributeBase::~FunctionAttributeBase() = default;


bool FunctionAttributeBase::operator==(const FunctionAttributeInterface &rhs) const noexcept {
  return (
    Name() == rhs.Name()
    && Parameters() == rhs.Parameters()
    && Scope() == rhs.Scope()
    && CvQualifier() == rhs.CvQualifier()
  );
}

bool FunctionAttributeBase::operator!=(const FunctionAttributeInterface &rhs) const noexcept {
  return !(*this == rhs);
}


bool FunctionAttributeBase::InScopeOf(const FunctionScope &scope) const noexcept {
  return Scope().In(scope);
}


std::string FunctionAttributeBase::ConstantExpression() const noexcept {
  if (info_.is_constexpr)
    return "constexpr";
  if (info_.is_consteval)
    return "consteval";
  return "";
}


std::string FunctionAttributeBase::DefinitionSuffix() const noexcept {
  // default will support...
  if (info_.is_deleted)
    return "deleted";
  return "";
}


std::string FunctionAttributeBase::ExceptionSuffix() const noexcept {
  // throw will support...
  if (info_.is_noexcept)
    return "noexcept";
  return "";
}


FunctionScope FunctionAttributeBase::Scope() const noexcept {
  return FunctionScope(info_.scope);
}


std::string FunctionAttributeBase::Name() const noexcept {
  return info_.name;
}


std::string FunctionAttributeBase::ReturnType() const noexcept {
  return info_.return_type;
}


std::string FunctionAttributeBase::ParameterList() const noexcept {
  return info_.signature;
}


std::vector<std::string> FunctionAttributeBase::Parameters() const noexcept {
  return info_.parameters;
}



MemberFunctionAttribute::MemberFunctionAttribute(const MemberFunctionInfo &info)
  : FunctionAttributeBase(info.base)
  , info_(info) {
 }


MemberFunctionAttribute::~MemberFunctionAttribute() = default;


bool MemberFunctionAttribute::IsClassMember() const noexcept {
  return true;
}


std::string MemberFunctionAttribute::AccessSpecifier() const noexcept {
  if (info_.access_specifier == MemberFunctionInfo::AccessSpecifier::kPublic) {
    return "public";
  }
  if (info_.access_specifier == MemberFunctionInfo::AccessSpecifier::kPrivate) {
    return "private";
  }
  if (info_.access_specifier == MemberFunctionInfo::AccessSpecifier::kProtected) {
    return "protected";
  }
  return "";
}


std::string MemberFunctionAttribute::CvQualifier() const noexcept {
  if (!info_.is_const && !info_.is_volatile) {
    return "";
  }
  if (info_.is_const && !info_.is_volatile) {
    return "const";
  }
  if (!info_.is_const && info_.is_volatile) {
    return "volatile";
  }
  return "const volatile";
}


bool MemberFunctionAttribute::IsAbleToPolymorphic() const noexcept {
  return info_.is_polymorphic;
}


std::vector<std::string> MemberFunctionAttribute::BaseClasses() const noexcept {
  return info_.base_classes;
}


std::string MemberFunctionAttribute::StorageClass() const noexcept {
  return "";
}


MemberFunctionInfo MemberFunctionAttribute::GetAttribute() {
  return info_;
}



NamespaceFunctionAttribute::NamespaceFunctionAttribute(const FunctionInfo &info)
  : FunctionAttributeBase(info.base)
  , info_(info) {
 }


NamespaceFunctionAttribute::~NamespaceFunctionAttribute() = default;


bool NamespaceFunctionAttribute::IsClassMember() const noexcept {
  return false;
}


std::string NamespaceFunctionAttribute::AccessSpecifier() const noexcept {
  return "";
}


std::string NamespaceFunctionAttribute::CvQualifier() const noexcept {
  return "";
}


bool NamespaceFunctionAttribute::IsAbleToPolymorphic() const noexcept {
  return false;
}


std::vector<std::string> NamespaceFunctionAttribute::BaseClasses() const noexcept {
  return {};
}

std::string NamespaceFunctionAttribute::StorageClass() const noexcept {
  if (info_.is_static)
    return "static";
  if (info_.is_extern)
    return "extern";
  return "";
}


FunctionInfo NamespaceFunctionAttribute::GetAttribute() {
  return info_;
}

