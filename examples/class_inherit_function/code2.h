#pragma once
#include <iostream>
#include <string>

class hoge {
 public:
  hoge() = default;
  virtual ~hoge() = default;
  virtual int class_inherit_function_bar(int a, const std::string b) const = 0;
};
