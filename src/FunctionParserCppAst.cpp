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
  base.is_noexcept = func.noexcept_condition() != type_safe::nullopt;
  // constexpr
  base.is_constexpr = func.is_constexpr();
  // consteval
  base.is_consteval = func.is_consteval();
  return base;
}

}  // namespace

FunctionParserCppAst::FunctionParserCppAst(const std::vector<std::string> &file_paths,
                                           const std::string &compile_database_path)
    : CodeParserCppAst(
          std::move(file_paths),
          cppast::whitelist<cppast::cpp_entity_kind::function_t, cppast::cpp_entity_kind::member_function_t>(),
          std::move(compile_database_path)) {}

FunctionParserCppAst::~FunctionParserCppAst() = default;

std::vector<FunctionInfo> FunctionParserCppAst::GetFunctionInfos() {
  if (!ready_) return {};

  std::vector<FunctionInfo> infos;
  for (auto &file : p_parser_->files()) {
    cppast::visit(file, filter_, [&infos](const cppast::cpp_entity &e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      // type handling
      if (e.kind() == cppast::cpp_function::kind()) {
        auto &func = reinterpret_cast<const cppast::cpp_function &>(e);
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
      return true;
    });
  }
  return infos;
}

std::vector<MemberFunctionInfo> FunctionParserCppAst::GetMemberFunctionInfos() {
  if (!ready_) return {};

  std::vector<MemberFunctionInfo> infos;
  for (auto &file : p_parser_->files()) {
    cppast::visit(file, filter_, [&infos](const cppast::cpp_entity &e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      // type handling
      if (e.kind() == cppast::cpp_member_function::kind()) {
        auto &func = reinterpret_cast<const cppast::cpp_member_function &>(e);
        MemberFunctionInfo function_info = {};
        // function base
        function_info.base = GetFunctionBase(func);
        // class_name
        function_info.class_name = func.semantic_scope();
        auto it_delimiter = func.semantic_scope().find("::");
        if (it_delimiter != std::string::npos) {
          function_info.class_name = func.semantic_scope().substr(0, it_delimiter);
        }
        // const
        function_info.is_const =
            (func.cv_qualifier() == cppast::cpp_cv_const || func.cv_qualifier() == cppast::cpp_cv_const_volatile);
        // polymorphic
        function_info.is_polymorphic = func.virtual_info() != type_safe::nullopt;

        infos.push_back(function_info);
        // } else if (e.kind() == cppast::cpp_function::kind()) {
        //   // auto& func = reinterpret_cast<const cppast::cpp_function &>(e);
      }
      return true;
    });
  }
  return infos;
}
