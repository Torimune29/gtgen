#include "gtgen.h"
#include "CodeAnalyzerInterface.h"
#include "FunctionAnalyzerCppAst.h"

namespace gtgen {

std::shared_ptr<CodeAnalyzerInterface> CreateAnalyzer(const std::vector<std::string> &file_paths, const std::string &compile_database_path, bool verbose) {
  std::shared_ptr<CodeParserCppAst> parser(new CodeParserCppAst(file_paths, compile_database_path, verbose));
  if (!parser->Ready())
    return nullptr;
  std::shared_ptr<FunctionAnalyzerCppAst> function_analyzer(new FunctionAnalyzerCppAst(parser));
  std::shared_ptr<CodeAnalyzerResultStorage> storage(new CodeAnalyzerResultStorage());
  storage->AddAnalyzer(function_analyzer);
  return std::move(storage);
}

}  // namespace gtgen


