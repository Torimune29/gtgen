#include "FunctionParserCppAst.h"

#include <cppast/cpp_class.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_function.hpp>
#include <iostream>
#include <type_safe/optional_ref.hpp>
#include <type_traits>

#include "FunctionInfo.h"

namespace {

template <typename T>
FunctionBase GetFunctionBase(const T &func) {
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
  return base;
}

}  // namespace

FunctionParserCppAst::FunctionParserCppAst(const std::vector<std::string> &file_paths,
                                           const std::string &compile_database_path)
    : CodeParserCppAst(std::move(file_paths),
                       cppast::whitelist<cppast::cpp_entity_kind::function_t, cppast::cpp_entity_kind::class_t>(),
                       std::move(compile_database_path)) {}

FunctionParserCppAst::~FunctionParserCppAst() = default;

std::vector<FunctionInfo> FunctionParserCppAst::GetFunctionInfos() {
  if (!ready_) return {};

  std::vector<FunctionInfo> infos;
  for (const auto &file : p_parser_->files()) {
    cppast::visit(file, filter_, [&infos](const cppast::cpp_entity &e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      // type handling
      if (e.kind() == cppast::cpp_function::kind()) {
        const auto &func = reinterpret_cast<const cppast::cpp_function &>(e);
        if (func.is_declaration()) {  // use declaration only
          FunctionInfo function_info = {};
          // function base
          function_info.base = GetFunctionBase(func);
          // class_name
          function_info.namespace_name = func.semantic_scope();
          auto it_delimiter = func.semantic_scope().find("::");
          if (it_delimiter != std::string::npos) {
            function_info.namespace_name = func.semantic_scope().substr(0, it_delimiter);
          }
          // extern
          function_info.is_extern = (func.storage_class() == cppast::cpp_storage_class_extern);
          // static
          function_info.is_static = (func.storage_class() == cppast::cpp_storage_class_static);

          infos.push_back(function_info);
        }
      }
      return true;
    });
  }
  return infos;
}

std::vector<MemberFunctionInfo> FunctionParserCppAst::GetMemberFunctionInfos() {
  if (!ready_) return {};

  std::vector<MemberFunctionInfo> infos;
  for (const auto &file : p_parser_->files()) {
    cppast::visit(file, filter_, [&infos](const cppast::cpp_entity &e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      // type handling
      if (e.kind() == cppast::cpp_class::kind()) {
        const auto &class_e = reinterpret_cast<const cppast::cpp_class &>(e);
        const auto class_name = class_e.name();
        auto access_specifier =
            (class_e.class_kind() == cppast::cpp_class_kind::class_t ? cppast::cpp_private : cppast::cpp_public);
        for (auto &&child : class_e) {
          switch (child.kind()) {
            case cppast::cpp_entity_kind::access_specifier_t: {
              access_specifier = reinterpret_cast<const cppast::cpp_access_specifier &>(child).access_specifier();
              break;
            }
            case cppast::cpp_entity_kind::member_function_t: {
              // if (!public_method) break;
              const auto &func = reinterpret_cast<const cppast::cpp_member_function &>(child);
              MemberFunctionInfo function_info = {};
              // function base
              function_info.base = GetFunctionBase(func);
              // access specifier
              if (access_specifier == cppast::cpp_public)
                function_info.access_specifier = MemberFunctionInfo::AccessSpecifier::Public;
              else if (access_specifier == cppast::cpp_private)
                function_info.access_specifier = MemberFunctionInfo::AccessSpecifier::Private;
              else if (access_specifier == cppast::cpp_protected)
                function_info.access_specifier = MemberFunctionInfo::AccessSpecifier::Protected;
              // class_name
              function_info.class_name = class_name;
              // const
              function_info.is_const =
                  (func.cv_qualifier() == cppast::cpp_cv_const || func.cv_qualifier() == cppast::cpp_cv_const_volatile);
              // polymorphic
              function_info.is_polymorphic = func.virtual_info() != type_safe::nullopt;
              infos.push_back(function_info);
              break;
            }
            default: {
              break;
            }
          }
        }
      }
      return true;
    });
  }
  return infos;
}
