#include "CodeParserCppAst.h"

#include <cppast/cpp_class.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_function.hpp>
#include <iostream>
#include <type_traits>

#include "FunctionInfo.h"

namespace cppast {
/**
 * @brief cppast logger for log suppression
 * @details
 * clang warns -Wweak-vtables because cppast defines virtual method on header files.
 * So set -Wno~ in cmake/external/cppast.cmake .
 *
 */
struct NoneLogger : diagnostic_logger {
  bool do_log(const char* /* source */, const diagnostic& /* d */) const override {
    return true;
  }
};

}  // namespace cppast


const char kSettingsNameCompileDatabase[] = "compile_database_path";
const char kSettingsVerbose[] = "verbose";

CodeParserCppAst::CodeParserCppAst(const std::vector<std::string> &file_paths, const FilterType &filter,
                                   const std::string &compile_database_path, bool verbose)
    : AbstractCodeParser(std::move(file_paths)), filter_(filter), ready_(false) {
  settings_.insert({kSettingsNameCompileDatabase, compile_database_path});
  settings_.insert({kSettingsVerbose, (verbose ? "true" : "false")});
  if (verbose) {
    p_logger_ = std::unique_ptr<LoggerType>(new cppast::stderr_diagnostic_logger(verbose));
  } else {
    p_logger_ = std::unique_ptr<LoggerType>(new cppast::NoneLogger());
  }
  p_parser_ = std::unique_ptr<ParserType>(new ParserType(type_safe::ref(index_), type_safe::ref(*p_logger_)));
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
