#pragma once

#include <memory>
#include <string>
#include <vector>

#include "CodeAnalyzerInterface.h"
#include "CodeParserCppAst.h"
#include "FunctionInfo.h"
#include "ScopeInfo.h"

class ScopeRelationParserImpl;

/**
 * @brief  Code ParserCppAst Class
 *
 */
class ScopeRelationAnalyzerCppAst :
  public CodeAnalyzerInterface , public std::enable_shared_from_this<CodeAnalyzerInterface> {
 public:
  explicit ScopeRelationAnalyzerCppAst(std::shared_ptr<CodeParserCppAst> p_parser);
  ~ScopeRelationAnalyzerCppAst() override;

  std::vector<ScopeInfo> GetScopes() noexcept override;
 private:
  std::shared_ptr<CodeParserCppAst> p_parser_;

  std::vector<ScopeInfo> Get() noexcept;
  template<typename T>
  std::vector<ScopeInfo> ParseScopeRelation(const T &entity) const noexcept;
  template<typename T>
  std::vector<ScopeInfo> ParseNamespaceScopeRelation(const T &entity) const noexcept;
  template<typename T>
  std::vector<ScopeInfo> ParseClassScopeRelation(const T &entity, bool expect_global) const noexcept;

};
