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

  std::vector<std::shared_ptr<FunctionAttributeInterface>> Get() noexcept;
  std::vector<FunctionInfo> GetFunction() noexcept;
  std::vector<MemberFunctionInfo> GetMemberFunction() noexcept ;
 private:
  template<typename T>
  std::vector<FunctionInfo> ParseFunction(const T &entity) const noexcept;
  template<typename T>
  std::vector<MemberFunctionInfo> ParseMemberFunction(const T &entity) const noexcept;
};
