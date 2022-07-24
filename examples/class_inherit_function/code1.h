#pragma once
#include <iostream>
#include <string>

#include "code2.h"

class huga {
 public:
  explicit huga(hoge *a) : a_(a) {
  }
  int class_inherit_function_foo(int a) {
    return a_->class_inherit_function_bar(a, "");
  }

 private:
  hoge *a_;
};
