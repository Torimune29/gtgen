#include "FunctionAttributeDecorator.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>

#include "cppcodegen.h"

namespace {
const char kMockMethodName[] = "MOCK_METHOD";
const char kMockConstMethodName[] = "MOCK_CONST_METHOD";
const char kMockMethodNoexceptWorkaroundSuffix[] = "_noexcept";
const char kMockMethodOperatorWorkaroundPrefix[] = "operator_";

std::string CreateWorkaroundMethodDefinition(const std::shared_ptr<FunctionAttributeInterface> &p_if,
                                             const std::string &workaround_method_name) {
  cppcodegen::Block mock_definition_block(cppcodegen::definition_t, p_if->Declaration());
  std::string method_definition;
  method_definition += "return this->" + workaround_method_name;
  if (p_if->Parameters().empty()) {
    method_definition += "();";
  } else {
    method_definition += "(";
    for (const auto &it : p_if->Parameters()) {
      method_definition += it.second + ", ";
    }
    method_definition = method_definition.substr(0, method_definition.size() - 2);
    method_definition += ");";
  }
  mock_definition_block << method_definition;
  return ";\n" + mock_definition_block.Out();
}

}  // namespace

GoogleMockLegacyDecorator::GoogleMockLegacyDecorator(std::shared_ptr<FunctionAttributeInterface> p_if, bool public_only)
    : FunctionAttributeDecorator(p_if), public_only_(public_only) {
}

std::string GoogleMockLegacyDecorator::Declaration() const noexcept {
  if (p_if_->DefinitionSuffix() == "delete" ||
      (public_only_ && p_if_->IsClassMember() && p_if_->AccessSpecifier() != "public") ||
      p_if_->CvQualifier().find("volatile") != std::string::npos) {  // not support volatile method mocking
    return "";
  }
  std::string mock_method;
  std::string method_name = p_if_->Name();
  std::string method_definition;
  // workaround check
  if (p_if_->IsOverloadedOperator()) {  // operator direct mock is unsupported, and operator is be able to set noexcept
    method_name = kMockMethodOperatorWorkaroundPrefix;
    method_name += std::to_string(std::hash<std::string>()(p_if_->Name()));
    method_definition = CreateWorkaroundMethodDefinition(p_if_, method_name);
  } else if (p_if_->ExceptionSuffix() == "noexcept") {  // noexcept direct mock is unsupported
    method_name += kMockMethodNoexceptWorkaroundSuffix;
    method_definition = CreateWorkaroundMethodDefinition(p_if_, method_name);
  } else {  // no definition
    method_definition = ";";
  }
  if (p_if_->CvQualifier().find("const") != std::string::npos) {
    mock_method += std::string(kMockConstMethodName);
  } else {
    mock_method += std::string(kMockMethodName);
  }
  mock_method += std::to_string(p_if_->ParameterTypes().size()) + "(" + method_name + ", " + p_if_->ReturnType() +
                 p_if_->Signature() + ")";
  mock_method += method_definition;
  return mock_method;
}
