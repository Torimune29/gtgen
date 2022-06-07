#include "CodeParserCppAst.h"

#include <cppast/cpp_class.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_function.hpp>
#include <iostream>
#include <type_traits>

#include "FunctionInfo.h"


const char kSettingsNameCompileDatabase[] = "compile_database_path";
const char kSettingsVerbose[] = "verbose";

CodeParserCppAst::CodeParserCppAst(const std::vector<std::string> &file_paths, const FilterType &filter,
                                   const std::string &compile_database_path, bool verbose)
    : AbstractCodeParser(std::move(file_paths)), filter_(filter), ready_(false) {
  settings_.insert({kSettingsNameCompileDatabase, compile_database_path});
  settings_.insert({kSettingsVerbose, (verbose ? "true" : "false")});
  logger_.set_verbose(verbose);
  p_parser_ = std::unique_ptr<ParserType>(new ParserType(type_safe::ref(index_), type_safe::ref(logger_)));
}

CodeParserCppAst::~CodeParserCppAst() = default;

bool CodeParserCppAst::Ready() noexcept {
  if (ready_) return ready_;
  try {
    p_database_ = std::unique_ptr<DatabaseType>(new DatabaseType(settings_[kSettingsNameCompileDatabase]));
    cppast::parse_files(*p_parser_, file_paths_, *p_database_);
    ready_ = true;
  } catch (cppast::libclang_error &ex) {
    std::cerr << "fatal libclang error: " << ex.what() << '\n';
  }

  return ready_;
}
