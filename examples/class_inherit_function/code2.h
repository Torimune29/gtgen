#pragma once
#include <iostream>
#include <string>

class hogehoge {
 public:
  hogehoge() = default;
  virtual ~hogehoge() = default;
  virtual int class_inherit_function_bar(int a, const std::string b) const = 0;
};
