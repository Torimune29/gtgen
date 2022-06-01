#pragma once

#include <string>

/**
 * @brief Abstract Code Parser Class
 *
 */
class AbstractCodeParser {
 public:
  AbstractCodeParser(const std::string &file_path, const std::string &compile_database_path);
  virtual ~AbstractCodeParser();

  virtual bool Ready();

  std::string GetFilePath() const;
  std::string GetCompileDatabasePath() const;

 protected:
  std::string path_;
  std::string compile_database_path_;
};
