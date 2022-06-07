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
  virtual ~AbstractCodeParser();
  virtual bool Ready() = 0;

  std::vector<std::string> GetFilePaths() const;
  std::unordered_map<std::string, std::string> GetSettings() const;

 protected:
  AbstractCodeParser() = default;
  explicit AbstractCodeParser(const std::vector<std::string> &file_paths);

  std::vector<std::string> file_paths_;
  std::unordered_map<std::string, std::string> settings_;
};
