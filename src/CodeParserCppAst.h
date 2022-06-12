#pragma once

#include "AbstractCodeParser.h"
#include <string>
#include <memory>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>
#include <cppast/diagnostic.hpp>

/**
 * @brief  Code ParserCppAst Class
 *
 */
class CodeParserCppAst : public AbstractCodeParser {
  using DatabaseType = cppast::libclang_compilation_database;
  using ParserType = cppast::simple_file_parser<cppast::libclang_parser>;
  using ResultTypeIndex = cppast::cpp_entity_index;
  using FilterType = cppast::detail::visitor_filter_t;
  using EntityType = cppast::cpp_entity;
  using LoggerType = cppast::diagnostic_logger;

 public:
  CodeParserCppAst(const std::vector<std::string> &file_paths,
                   const std::string &compile_database_path, bool verbose);
  ~CodeParserCppAst() override;

  bool Ready() noexcept final;
  const cppast::simple_file_parser<cppast::libclang_parser> &  GetParserRef() noexcept;
  static std::string GetFullName(const cppast::cpp_entity &e) noexcept;
  static std::vector<std::string> GetScopes(const cppast::cpp_entity &e) noexcept;
  void Log(const std::string &label, const std::string &message, cppast::severity severity) const noexcept;

 protected:
  ResultTypeIndex index_;
  std::unique_ptr<ParserType> p_parser_;
  std::unique_ptr<DatabaseType> p_database_;
  std::vector<EntityType> entities_;
  std::unique_ptr<LoggerType> p_logger_;
  bool ready_;
};
