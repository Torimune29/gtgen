#pragma once

#include "CodeParserCppAst.h"
#include "FunctionInfo.h"
#include <string>
#include <memory>

/**
 * @brief  Code ParserCppAst Class
 *
 */
class FunctionParserCppAst : public CodeParserCppAst {
 public:
  FunctionParserCppAst(const std::vector<std::string> &file_paths, const std::string &compile_database_path);
  ~FunctionParserCppAst() override;

  std::vector<MemberFunctionInfo> GetFunctionInfos();
};
