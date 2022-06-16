#include "FunctionAnalyzerCppAst.h"

#include <algorithm>
#include <cppast/cpp_class.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_function.hpp>
#include <cppast/cpp_namespace.hpp>
#include <iostream>
#include <type_safe/optional_ref.hpp>

namespace {
cppast::detail::visitor_filter_t FunctionWhiteList() {
  return cppast::whitelist<cppast::cpp_entity_kind::function_t, cppast::cpp_entity_kind::class_t>();
}

template <typename T, typename U>
bool BodyEqual(const T &func, U kind) {
  return (func.body_kind() == kind && func.is_declaration() == cppast::is_declaration(kind) &&
          func.is_definition() == cppast::is_definition(kind));
}

template <typename T>
FunctionBase GetBase(const T &func) {
  FunctionBase base = {};
  // function name
  base.name = func.name();
  // return
  base.return_type = cppast::to_string(func.return_type());
  // signature
  base.signature = func.signature();
  auto it_suffix = func.signature().find(") ");
  if (it_suffix != std::string::npos) {
    base.signature = func.signature().substr(0, it_suffix + 1);
  }
  // parameter_types
  for (auto &it : func.parameters()) {
    base.parameter_types.push_back(cppast::to_string(it.type()));
    base.parameters.push_back(std::make_pair(cppast::to_string(it.type()), it.name()));
  }
  // noexcept
  if (func.noexcept_condition() != type_safe::nullopt &&
      func.noexcept_condition().value().kind() == cppast::cpp_expression_kind::literal_t) {
    base.is_noexcept =
        (reinterpret_cast<const cppast::cpp_literal_expression &>(func.noexcept_condition().value()).value() == "true");
  }
  // constexpr
  base.is_constexpr = func.is_constexpr();
  // consteval
  base.is_consteval = func.is_consteval();
  // variadic like args...
  base.is_variadic = func.is_variadic();
  // deleted
  base.is_deleted = BodyEqual(func, cppast::cpp_function_deleted);
  return base;
}

}  // namespace

FunctionAnalyzerCppAst::FunctionAnalyzerCppAst(std::shared_ptr<CodeParserCppAst> p_parser) : p_parser_(p_parser) {
}

FunctionAnalyzerCppAst::~FunctionAnalyzerCppAst() = default;

std::vector<std::shared_ptr<FunctionAttributeInterface>> FunctionAnalyzerCppAst::GetFunctions() noexcept {
  auto functions = GetFunction();
  auto member_functions = GetMemberFunction();
  std::vector<std::shared_ptr<FunctionAttributeInterface>> v;
  std::for_each(functions.cbegin(), functions.cend(),
                [&v](const FunctionInfo &info) { v.emplace_back(std::make_shared<NamespaceFunctionAttribute>(info)); });
  std::for_each(member_functions.cbegin(), member_functions.cend(), [&v](const MemberFunctionInfo &info) {
    v.emplace_back(std::make_shared<MemberFunctionAttribute>(info));
  });
  return v;
}

std::vector<FunctionInfo> FunctionAnalyzerCppAst::GetFunction() noexcept {
  std::vector<FunctionInfo> infos;
  for (const auto &file : p_parser_->GetParserRef().files()) {
    cppast::visit(file, FunctionWhiteList(), [&](const cppast::cpp_entity &e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      // type handling
      auto infos_parsed = ParseFunction(e);
      infos.insert(infos.end(), infos_parsed.begin(), infos_parsed.end());
      return true;
    });
  }
  return infos;
}

std::vector<MemberFunctionInfo> FunctionAnalyzerCppAst::GetMemberFunction() noexcept {
  std::vector<MemberFunctionInfo> infos;
  for (const auto &file : p_parser_->GetParserRef().files()) {
    cppast::visit(file, FunctionWhiteList(), [&](const cppast::cpp_entity &e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      auto infos_parsed = ParseMemberFunction(e);
      infos.insert(infos.end(), infos_parsed.begin(), infos_parsed.end());
      return true;
    });
  }
  return infos;
}

template <typename T>
std::vector<FunctionInfo> FunctionAnalyzerCppAst::ParseFunction(const T &entity) const noexcept {
  std::vector<FunctionInfo> infos;
  if (entity.kind() == cppast::cpp_function::kind()) {
    const auto &func = reinterpret_cast<const cppast::cpp_function &>(entity);
    FunctionInfo function_info = {};
    // function base
    function_info.base = GetBase(func);
    // extern
    function_info.is_extern = (func.storage_class() == cppast::cpp_storage_class_extern);
    // static
    function_info.is_static = (func.storage_class() == cppast::cpp_storage_class_static);
    // scope
    function_info.base.scope = p_parser_->GetScopes(func.parent().value());
    infos.push_back(function_info);
  }
  return infos;
}

template <typename T>
std::vector<MemberFunctionInfo> FunctionAnalyzerCppAst::ParseMemberFunction(const T &entity) const noexcept {
  std::vector<MemberFunctionInfo> infos;
  if (entity.kind() == cppast::cpp_class::kind()) {
    const auto &class_e = reinterpret_cast<const cppast::cpp_class &>(entity);
    const auto class_name = class_e.name();

    p_parser_->Log("MemberFunction Class in:", class_name, cppast::severity::debug);

    auto access_specifier =
        (class_e.class_kind() == cppast::cpp_class_kind::class_t ? cppast::cpp_private : cppast::cpp_public);
    const auto base_classes_raw = class_e.bases();
    std::vector<std::string> base_classes;
    for (auto &it : base_classes_raw) {
      base_classes.push_back(it.name());
    }
    for (auto &&child : class_e) {
      switch (child.kind()) {
        case cppast::cpp_entity_kind::access_specifier_t: {
          access_specifier = reinterpret_cast<const cppast::cpp_access_specifier &>(child).access_specifier();
          break;
        }
        case cppast::cpp_entity_kind::member_function_t: {
          const auto &func = reinterpret_cast<const cppast::cpp_member_function &>(child);

          p_parser_->Log("MemberFunction Function in:", func.name(), cppast::severity::debug);

          MemberFunctionInfo function_info = {};
          // function base
          function_info.base = GetBase(func);
          // access specifier
          if (access_specifier == cppast::cpp_public)
            function_info.access_specifier = MemberFunctionInfo::AccessSpecifier::kPublic;
          else if (access_specifier == cppast::cpp_private)
            function_info.access_specifier = MemberFunctionInfo::AccessSpecifier::kPrivate;
          else if (access_specifier == cppast::cpp_protected)
            function_info.access_specifier = MemberFunctionInfo::AccessSpecifier::kProtected;
          // class_name
          function_info.class_name = class_name;
          // const
          function_info.is_const =
              (func.cv_qualifier() == cppast::cpp_cv_const || func.cv_qualifier() == cppast::cpp_cv_const_volatile);
          // const
          function_info.is_volatile =
              (func.cv_qualifier() == cppast::cpp_cv_volatile || func.cv_qualifier() == cppast::cpp_cv_const_volatile);
          // polymorphic
          function_info.is_polymorphic = func.virtual_info() != type_safe::nullopt;
          // base classes
          function_info.base_classes = base_classes;
          // scope
          function_info.base.scope = p_parser_->GetScopes(func.parent().value());
          if (function_info.base.scope.empty())
            function_info.base.scope = std::vector<std::string>({class_name});
          else
            function_info.base.scope.push_back(class_name);

          infos.push_back(function_info);

          p_parser_->Log("MemberFunction Function out:", func.name(), cppast::severity::debug);

          break;
        }
        default: {
          break;
        }
      }
    }

    p_parser_->Log("MemberFunction Class out:", class_name, cppast::severity::debug);
  }
  return infos;
}
