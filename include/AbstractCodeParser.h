#pragma once

#include <string>
#include <vector>
#include <unordered_map>

/**
 * @brief Abstract Code Parser Class
 *
 */
class AbstractCodeParser {
 public:
  explicit AbstractCodeParser(const std::vector<std::string> &file_paths);
  virtual ~AbstractCodeParser();

  virtual bool Ready();

  std::vector<std::string> GetFilePaths() const;
  std::unordered_map<std::string, std::string> GetSettings() const;

 protected:
  std::vector<std::string> file_paths_;
  std::unordered_map<std::string, std::string> settings_;
};
