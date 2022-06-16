#include "FunctionAttributeDecorator.h"

#include <algorithm>
#include <iostream>
#include <memory>

const char kMockMethodName[] = "MOCK_METHOD";
const char kMockConstMethodName[] = "MOCK_CONST_METHOD";
const char kMockMethodNoexceptWorkaroundSuffix[] = "_noexcept";
GoogleMockLegacyDecorator::GoogleMockLegacyDecorator(std::shared_ptr<FunctionAttributeInterface> p_if, bool public_only)
    : FunctionAttributeDecorator(p_if), public_only_(public_only) {
}

std::string GoogleMockLegacyDecorator::Declaration() const noexcept {
  if (p_if_->DefinitionSuffix() == "deleted" ||
      (public_only_ && p_if_->IsClassMember() && p_if_->AccessSpecifier() != "public") ||
      p_if_->CvQualifier().find("volatile") != std::string::npos) {  // not support volatile method mocking
    return "";
  }
  std::string mock_method;
  std::string method_name = p_if_->Name();
  std::string method_definition;
  // noexcept workaround check
  if (p_if_->ExceptionSuffix() == "noexcept") {
    method_name += kMockMethodNoexceptWorkaroundSuffix;
    method_definition = ";\n  " + p_if_->Declaration() + " {\n";
    method_definition += "    return this->" + method_name;
    if (p_if_->Parameters().empty()) {
      method_definition += "();\n";
    } else {
      method_definition += "(";
      for (const auto &it : p_if_->Parameters()) {
        method_definition += it.second + ", ";
      }
      method_definition = method_definition.substr(0, method_definition.size() - 2);
      method_definition += ");\n";
    }
    method_definition += "  }\n";
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
