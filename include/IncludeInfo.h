#pragma once

#include <string>

typedef struct IncludeInfo {
  enum class Kind { kSystem, kLocal } kind;
  std::string name;
  std::string full_path;
} IncludeInfo;
