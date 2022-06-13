#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "CodeAnalyzerInterface.h"

class CodeAnalyzerInterface;
namespace gtgen {
std::shared_ptr<CodeAnalyzerInterface> CreateAnalyzer(const std::vector<std::string> &file_paths,
                                                      const std::string &compile_database_path, bool verbose);
}  // namespace gtgen
