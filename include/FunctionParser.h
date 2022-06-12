#pragma once

#include <memory>
#include <string>
#include <vector>

#include "AbstractCodeParser.h"
#include "FunctionInfo.h"

class FunctionParserImpl;

/**
 * @brief  Code ParserCppAst Class
 *
 */
class FunctionParser : public AbstractCodeParser {
 public:
  FunctionParser(const std::vector<std::string> &file_paths, const std::string &compile_database_path, bool verbose);
  ~FunctionParser() override;

  bool Ready() noexcept final;

  std::vector<std::shared_ptr<FunctionAttributeInterface>> Get() noexcept;
  std::vector<FunctionInfo> GetFunction() noexcept;
  std::vector<MemberFunctionInfo> GetMemberFunction() noexcept;

 private:
  std::unique_ptr<FunctionParserImpl> p_impl_;
};
