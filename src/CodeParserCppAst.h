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
  ~CodeParserCppAst() override;

  bool Ready() noexcept final;

 protected:
  CodeParserCppAst(const std::vector<std::string> &file_paths, const FilterType &filter,
                   const std::string &compile_database_path, bool verbose);
  std::string GetFullName(const cppast::cpp_entity &e) const noexcept;

  ResultTypeIndex index_;
  std::unique_ptr<ParserType> p_parser_;
  std::unique_ptr<DatabaseType> p_database_;
  FilterType filter_;
  std::vector<EntityType> entities_;
  std::unique_ptr<LoggerType> p_logger_;
  bool ready_;
};
