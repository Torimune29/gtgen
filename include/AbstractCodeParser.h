#pragma once

#include <string>

/**
 * @brief Abstract Code Parser Class
 *
 */
class AbstractCodeParser {
 public:
  explicit AbstractCodeParser(const std::string &path);
  virtual ~AbstractCodeParser();

  std::string GetPath() const;

 protected:
  std::string path_;
};
