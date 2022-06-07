#pragma once

#include <memory>
#include <string>

#include "CodeParserCppAst.h"
#include "FunctionInfo.h"

/**
 * @brief  Code ParserCppAst Class
 *
 */
class FunctionParserImpl : public CodeParserCppAst {
 public:
  FunctionParserImpl(const std::vector<std::string> &file_paths, const std::string &compile_database_path,
                     bool verbose);
  ~FunctionParserImpl() override;

  std::vector<FunctionInfo> GetFunction();
  std::vector<MemberFunctionInfo> GetMemberFunction();
};
