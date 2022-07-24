#pragma once
#include <iostream>
#include <string>

#include "code2.h"

bool namespace_function_foo(int a) {
  return hoge::namespace_function_bar(a, "") > 0;
}
