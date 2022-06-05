#include "AbstractCodeParser.h"


AbstractCodeParser::AbstractCodeParser(const std::vector<std::string> &file_paths)
  : file_paths_(std::move(file_paths)) {
}


AbstractCodeParser::~AbstractCodeParser() = default;


std::vector<std::string> AbstractCodeParser::GetFilePaths() const {
  return file_paths_;
}


std::unordered_map<std::string, std::string> AbstractCodeParser::GetSettings() const {
  return settings_;
}

