#include "AbstractCodeParser.h"

AbstractCodeParser::AbstractCodeParser(const std::string &path)
  : path_(std::move(path)) {
}


AbstractCodeParser::~AbstractCodeParser() = default;


std::string AbstractCodeParser::GetPath() const {
  return path_;
}

