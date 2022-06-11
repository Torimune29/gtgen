#pragma once

#include <memory>
#include <string>

#include "CodeParserCppAst.h"
#include "ScopeInfo.h"

/**
 * @brief  Code ParserCppAst Class
 *
 */
class ScopeRelationParserImpl : public CodeParserCppAst {
 public:
  ScopeRelationParserImpl(const std::vector<std::string> &file_paths, const std::string &compile_database_path,
                     bool verbose);
  ~ScopeRelationParserImpl() override;

  std::vector<ScopeInfo> Get() noexcept;

 private:
  template<typename T>
  std::vector<ScopeInfo> ParseScopeRelation(const T &entity) const noexcept;
  template<typename T>
  std::vector<ScopeInfo> ParseNamespaceScopeRelation(const T &entity) const noexcept;
  template<typename T>
  std::vector<ScopeInfo> ParseClassScopeRelation(const T &entity, bool expect_global) const noexcept;
};
