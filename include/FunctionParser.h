#pragma once

#include <memory>
#include <string>

#include "CodeParserCppAst.h"
#include "FunctionInfo.h"

/**
 * @brief  Code ParserCppAst Class
 *
 */
class FunctionParser : public CodeParserCppAst {
 public:
  FunctionParser(const std::vector<std::string> &file_paths, const std::string &compile_database_path, bool verbose);
  FunctionParser(const std::string &class_name, const std::vector<std::string> &file_paths, const std::string &compile_database_path, bool verbose);
  ~FunctionParser() override;

  std::vector<FunctionInfo> GetFunctionInfos();
  std::vector<MemberFunctionInfo> GetMemberFunctionInfos();
 private:
  const std::string class_name_;
};
