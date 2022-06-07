#pragma once

#include <memory>
#include <string>

#include "FunctionInfo.h"
#include "CodeParserCppAst.h"

/**
 * @brief  Code ParserCppAst Class
 *
 */
class FunctionParserImpl : public CodeParserCppAst {
 public:
  FunctionParserImpl(const std::vector<std::string> &file_paths, const std::string &compile_database_path, bool verbose);
  FunctionParserImpl(const std::string &class_name, const std::vector<std::string> &file_paths, const std::string &compile_database_path, bool verbose);
  ~FunctionParserImpl() override;

  std::vector<FunctionInfo> GetFunctionInfos();
  std::vector<MemberFunctionInfo> GetMemberFunctionInfos();
 private:
  const std::string class_name_;
};
