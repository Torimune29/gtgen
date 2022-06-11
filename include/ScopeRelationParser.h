#pragma once

#include <memory>
#include <string>
#include <vector>

#include "AbstractCodeParser.h"
#include "ScopeInfo.h"

class ScopeRelationParserImpl;

/**
 * @brief  Code ParserCppAst Class
 *
 */
class ScopeRelationParser : public AbstractCodeParser {
 public:
  ScopeRelationParser(const std::vector<std::string> &file_paths, const std::string &compile_database_path, bool verbose);
  ~ScopeRelationParser() override;

  bool Ready() noexcept final;

  std::vector<ScopeInfo> Get() noexcept;

 private:
  std::unique_ptr<ScopeRelationParserImpl> p_impl_;
};
