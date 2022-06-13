#pragma once

#include <memory>
#include <string>
#include <vector>

#include "CodeAnalyzerInterface.h"
#include "CodeParserCppAst.h"

/**
 * @brief  Code ParserCppAst Class
 *
 */
class IncludeAnalyzerCppAst : public CodeAnalyzerInterface, public std::enable_shared_from_this<CodeAnalyzerInterface> {
 public:
  explicit IncludeAnalyzerCppAst(std::shared_ptr<CodeParserCppAst> p_parser);
  ~IncludeAnalyzerCppAst() override;

  std::vector<IncludeInfo> GetIncludes() noexcept override;

 private:
  std::shared_ptr<CodeParserCppAst> p_parser_;
};
