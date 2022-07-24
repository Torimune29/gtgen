#pragma once
#include <iostream>
#include <string>

#include "code2.h"

bool class_function_foo(int a) {
  hoge hoge1;
  return hoge1.class_function_bar(a, "") > 0;
}
