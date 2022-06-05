#pragma once

#include "CodeParserCppAst.h"
#include <string>
#include <memory>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>

/**
 * @brief  Code ParserCppAst Class
 *
 */
class FunctionParserCppAst : public CodeParserCppAst {
 public:
  FunctionParserCppAst(const std::vector<std::string> &file_paths, const std::string &compile_database_path);
  ~FunctionParserCppAst() override;

  std::vector<std::string> GetFunctionNames();
};
