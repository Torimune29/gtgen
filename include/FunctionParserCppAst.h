#pragma once

#include <memory>
#include <string>

#include "CodeParserCppAst.h"
#include "FunctionInfo.h"

/**
 * @brief  Code ParserCppAst Class
 *
 */
class FunctionParserCppAst : public CodeParserCppAst {
 public:
  FunctionParserCppAst(const std::vector<std::string> &file_paths, const std::string &compile_database_path, bool verbose);
  ~FunctionParserCppAst() override;

  std::vector<FunctionInfo> GetFunctionInfos();
  std::vector<MemberFunctionInfo> GetMemberFunctionInfos();
};
