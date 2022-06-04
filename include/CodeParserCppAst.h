#pragma once

#include "AbstractCodeParser.h"
#include <string>
#include <memory>
#include <cppast/libclang_parser.hpp>

/**
 * @brief  Code ParserCppAst Class
 *
 */
class CodeParserCppAst : public AbstractCodeParser {
  using DatabaseType = cppast::libclang_compilation_database;
  using ParserType = cppast::simple_file_parser<cppast::libclang_parser>;
  using ResultTypeIndex = cppast::cpp_entity_index;

 public:
  CodeParserCppAst(const std::vector<std::string> &file_paths, const std::string &compile_database_path);
  ~CodeParserCppAst() override;

  bool Ready() final;
 private:
  ResultTypeIndex index_;
  std::unique_ptr<ParserType> p_parser_;
  std::unique_ptr<DatabaseType> p_database_;
  bool ready_;
};
