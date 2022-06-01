#pragma once

#include "AbstractCodeParser.h"
#include <string>
#include <cppast/libclang_parser.hpp>

/**
 * @brief  Code ParserCppAst Class
 *
 */
class CodeParserCppAst : public AbstractCodeParser {
 public:
  CodeParserCppAst(const std::string &file_path, const std::string &compile_database_path);
  ~CodeParserCppAst() override;

  bool Ready() final;
};
