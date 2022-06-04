#include "CodeParserCppAst.h"
#include <iostream>
#include <type_traits>
#include <cppast/visitor.hpp>


const char kSettingsNameCompileDatabase[] = "compile_database_path";

CodeParserCppAst::CodeParserCppAst(const std::vector<std::string> &file_paths, const std::string &compile_database_path)
  : AbstractCodeParser(std::move(file_paths))
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
  std::string prefix;
  for (auto &file: p_parser_->files()) {
    cppast::visit(file, [&prefix](const cppast::cpp_entity& e, cppast::visitor_info info) {
        if (info.event == cppast::visitor_info::container_entity_exit) // exiting an old container
            prefix.pop_back();
        else if (info.event == cppast::visitor_info::container_entity_enter) {// entering a new container
            std::cout << prefix << "'" << e.name() << "' - " << cppast::to_string(e.kind()) << '\n';
            prefix += "\t";
        }
        else // if (info.event == cppast::visitor_info::leaf_entity) // a non-container entity
            std::cout << prefix << "'" << e.name() << "' - " << cppast::to_string(e.kind()) << '\n';
    });
  }
  return {prefix};
}
