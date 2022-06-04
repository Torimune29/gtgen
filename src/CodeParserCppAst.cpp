#include "CodeParserCppAst.h"
#include "FunctionInfo.h"
#include <cppast/cpp_class.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_function.hpp>
#include <iostream>
#include <type_traits>


const char kSettingsNameCompileDatabase[] = "compile_database_path";

CodeParserCppAst::CodeParserCppAst(const std::vector<std::string> &file_paths, const std::string &compile_database_path)
  : AbstractCodeParser(std::move(file_paths))
  , filter_(cppast::whitelist<
    cppast::cpp_entity_kind::function_t,
    cppast::cpp_entity_kind::member_function_t,
    cppast::cpp_entity_kind::function_template_t,
    cppast::cpp_entity_kind::function_template_specialization_t
    >())
  , ready_(false) {
  settings_.insert({kSettingsNameCompileDatabase, compile_database_path});
  p_parser_ =  std::unique_ptr<ParserType>(new ParserType(type_safe::ref(index_)));
}


CodeParserCppAst::~CodeParserCppAst() = default;


bool CodeParserCppAst::Ready() {
  if (ready_) return ready_;
  try {
    p_database_ = std::unique_ptr<DatabaseType>(new DatabaseType(settings_[kSettingsNameCompileDatabase]));
    cppast::parse_files(*p_parser_, file_paths_, *p_database_);
    ready_ = true;
  } catch (cppast::libclang_error& ex) {
    std::cerr << "fatal libclang error: " << ex.what() << '\n';
  }


  return ready_;
}

std::vector<std::string> CodeParserCppAst::GetFunctionNames() {
  // std::string prefix;
  // for (auto &file: p_parser_->files()) {
  //   cppast::visit(file, filter_, [&prefix](const cppast::cpp_entity& e, cppast::visitor_info info) {
  //       if (info.event == cppast::visitor_info::container_entity_exit) // exiting an old container
  //           prefix.pop_back();
  //       else if (info.event == cppast::visitor_info::container_entity_enter) {// entering a new container
  //           std::cout << prefix << "'" << e.name() << "' - " << cppast::to_string(e.kind()) << '\n';
  //           prefix += "\t";
  //       }
  //       else // if (info.event == cppast::visitor_info::leaf_entity) // a non-container entity
  //           std::cout << prefix << "'" << e.name() << "' - " << cppast::to_string(e.kind()) << '\n';
  //   });
  // }
  // return {prefix};
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
  std::vector<std::string> ret;
  for (const auto &it : infos) {
    ret.push_back(it.base.name);
  }
  return ret;
}
