// This code was generated by gtgen 0.0.1
#pragma once
#include <gmock/gmock.h>
#include <iostream>
#include <string>

class mock_FreeFunction {
 public:
  ~mock_FreeFunction() = default;
  static mock_FreeFunction& GetInstance() {
    static mock_FreeFunction instance;
    return instance;
  }
  MOCK_METHOD2(free_function_bar, int(int,std::string const));
  
 private:
  mock_FreeFunction() = default;
};

int free_function_bar (int a, std::string const b) {
  return mock_FreeFunction::GetInstance().free_function_bar(a, b);
}

