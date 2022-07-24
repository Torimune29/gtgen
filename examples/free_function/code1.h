#pragma once
#include <iostream>
#include <string>

#include "code2.h"

bool free_function_foo(int a) {
  return free_function_bar(a, "") > 0;
}
