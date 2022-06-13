#include "gtgen.h"
#include "CodeAnalyzerInterface.h"
#include "CodeAnalyzerResultStorage.h"
#include "FunctionAnalyzerCppAst.h"
#include "ScopeRelationAnalyzerCppAst.h"
#include "IncludeAnalyzerCppAst.h"

namespace gtgen {

std::shared_ptr<CodeAnalyzerInterface> CreateAnalyzer(const std::vector<std::string> &file_paths, const std::string &compile_database_path, bool verbose) {
  std::shared_ptr<CodeParserCppAst> parser(new CodeParserCppAst(file_paths, compile_database_path, verbose));
  if (!parser->Ready())
    return nullptr;
  std::shared_ptr<CodeAnalyzerResultStorage> storage(new CodeAnalyzerResultStorage());
  storage->AddAnalyzer(std::make_shared<FunctionAnalyzerCppAst>(parser));
  storage->AddAnalyzer(std::make_shared<ScopeRelationAnalyzerCppAst>(parser));
  storage->AddAnalyzer(std::make_shared<IncludeAnalyzerCppAst>(parser));
  return std::move(storage);
}

}  // namespace gtgen


