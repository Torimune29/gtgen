#include "CodeParserCppAst.h"
#include <iostream>
#include <type_traits>


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

