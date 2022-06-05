#include "FunctionParserCppAst.h"
#include "FunctionInfo.h"
#include <cppast/cpp_class.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_function.hpp>
#include <iostream>
#include <type_traits>


FunctionParserCppAst::FunctionParserCppAst(const std::vector<std::string> &file_paths, const std::string &compile_database_path)
  : CodeParserCppAst(
    std::move(file_paths)
    , cppast::whitelist<
    cppast::cpp_entity_kind::function_t,
    cppast::cpp_entity_kind::member_function_t,
    cppast::cpp_entity_kind::function_template_t,
    cppast::cpp_entity_kind::function_template_specialization_t
    >()
    , std::move(compile_database_path)) {
}


FunctionParserCppAst::~FunctionParserCppAst() = default;


std::vector<MemberFunctionInfo> FunctionParserCppAst::GetFunctionInfos() {
  if (!ready_) return {};

  std::vector<MemberFunctionInfo> infos;
  for (auto &file: p_parser_->files()) {
    cppast::visit(file, filter_, [&infos](const cppast::cpp_entity& e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      // type handling
      if (e.kind() == cppast::cpp_member_function::kind()) {
        MemberFunctionInfo function_info = {};
        auto& func = reinterpret_cast<const cppast::cpp_member_function &>(e);
        function_info.base.name = func.name();
        infos.push_back(function_info);
      // } else if (e.kind() == cppast::cpp_function::kind()) {
      //   // auto& func = reinterpret_cast<const cppast::cpp_function &>(e);
      }
      return true;
    });
  }
  return infos;
}
