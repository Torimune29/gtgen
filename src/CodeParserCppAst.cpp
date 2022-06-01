#include "CodeParserCppAst.h"
#include <iostream>


CodeParserCppAst::CodeParserCppAst(const std::string &path, const std::string &compile_database_path)
  : AbstractCodeParser(std::move(path), std::move(compile_database_path)) {
}


CodeParserCppAst::~CodeParserCppAst() = default;


bool CodeParserCppAst::Ready() {
  cppast::cpp_entity_index index = {};
  cppast::libclang_compilation_database database(GetCompileDatabasePath()); // the compilation database
  cppast::simple_file_parser<cppast::libclang_parser> parser(type_safe::ref(index));
  try {
    cppast::parse_database(parser, database);
  } catch (cppast::libclang_error& ex) {
    std::cerr << "fatal libclang error: " << ex.what() << '\n';
    return false;
  }
  return true;
}
