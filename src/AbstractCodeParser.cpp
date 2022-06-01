#include "AbstractCodeParser.h"


AbstractCodeParser::AbstractCodeParser(const std::string &path, const std::string &compile_database_path)
  : path_(std::move(path))
  , compile_database_path_(std::move(compile_database_path)) {
}


AbstractCodeParser::~AbstractCodeParser() = default;


bool AbstractCodeParser::Ready() {
  return true;
}


std::string AbstractCodeParser::GetFilePath() const {
  return path_;
}

std::string AbstractCodeParser::GetCompileDatabasePath() const {
  return compile_database_path_;
}

