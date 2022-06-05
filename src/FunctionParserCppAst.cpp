#include "FunctionParserCppAst.h"

#include <cppast/cpp_class.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_function.hpp>
#include <iostream>
#include <type_safe/optional_ref.hpp>
#include <type_traits>

#include "FunctionInfo.h"

FunctionParserCppAst::FunctionParserCppAst(const std::vector<std::string> &file_paths,
                                           const std::string &compile_database_path)
    : CodeParserCppAst(
          std::move(file_paths),
          cppast::whitelist<cppast::cpp_entity_kind::function_t, cppast::cpp_entity_kind::member_function_t>(),
          std::move(compile_database_path)) {}

FunctionParserCppAst::~FunctionParserCppAst() = default;

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
        // function name
        function_info.base.name = func.name();
        // return
        function_info.base.return_type = "";
        // signature
        auto signature = func.signature();
        auto it_suffix = signature.find(") ");
        if (it_suffix != std::string::npos) {
          signature = signature.substr(0, it_suffix + 1);
        }
        function_info.base.signature = std::move(signature);
        // noexcept
        function_info.base.is_noexcept = func.noexcept_condition() != type_safe::nullopt;
        // constexpr
        function_info.base.is_constexpr = func.is_constexpr();
        // consteval
        function_info.base.is_consteval = func.is_consteval();
        // class_name
        auto semantic_scope = func.semantic_scope();
        auto it_delimiter = semantic_scope.find("::");
        if (it_delimiter != std::string::npos) {
          semantic_scope = semantic_scope.substr(0, it_delimiter);
        }
        function_info.class_name = std::move(semantic_scope);
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
