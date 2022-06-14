#include "CodeParserCppAst.h"

#include <cppast/cpp_class.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_function.hpp>
#include <deque>
#include <iostream>
#include <type_traits>
#include <vector>

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
  bool do_log(const char * /* source */, const diagnostic & /* d */) const override { return true; }
};

}  // namespace cppast

const char kSettingsNameCompileDatabase[] = "compile_database_directory_path";
const char kSettingsVerbose[] = "verbose";

CodeParserCppAst::CodeParserCppAst(const std::vector<std::string> &file_paths, const std::string &compile_database_path,
                                   bool verbose)
    : AbstractCodeParser(std::move(file_paths)), ready_(false) {
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

const cppast::simple_file_parser<cppast::libclang_parser> &CodeParserCppAst::GetParserRef() noexcept {
  return (*p_parser_);
}

std::string CodeParserCppAst::GetFullName(const cppast::cpp_entity &e) noexcept {
  std::string full_name;
  auto scopes = GetScopes(e);
  if (scopes.empty()) return "";
  for (const auto &it : scopes) {
    full_name += it + "::";
  }
  return full_name.substr(0, full_name.size() - 2);
}

std::vector<std::string> CodeParserCppAst::GetScopes(const cppast::cpp_entity &e) noexcept {
  if (e.name().empty()) {
    return {};
  }
  if (cppast::is_parameter(e.kind())) {  // parameters don't have a full name
    return {e.name()};
  }

  std::deque<std::string> scopes;

  for (auto cur = e.parent(); cur; cur = cur.value().parent()) {
    // prepend each scope, if there is any
    type_safe::with(cur.value().scope_name(), [&](const cppast::cpp_scope_name &cur_scope) {
      // if class, cur_scope includes class name at the end. Cut.
      if (e.kind() == cppast::cpp_entity_kind::class_t) {
        const auto &c = static_cast<const cppast::cpp_class &>(e);
        if (cur_scope.name() == c.name()) return;
      }
      if (!cur_scope.name().empty()) {
        scopes.push_front(cur_scope.name());
      }
    });
  }

  if (e.kind() == cppast::cpp_entity_kind::class_t) {
    const auto &c = static_cast<const cppast::cpp_class &>(e);
    if (!c.semantic_scope().empty()) scopes.push_back(c.semantic_scope());
  } else if (e.kind() != cppast::cpp_entity_kind::file_t) {
    // free function has file parent entity, so exclude
    scopes.push_back(e.name());
  }
  return std::vector<std::string>(scopes.begin(), scopes.end());
}

void CodeParserCppAst::Log(const std::string &label, const std::string &message,
                           cppast::severity severity) const noexcept {
  std::string full_message;
  full_message += label + " " + message;
  p_logger_->log("gtgen_CodeParser",
                 cppast::diagnostic{full_message, cppast::source_location::make_unknown(), severity});
}
