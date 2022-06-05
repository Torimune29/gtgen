#pragma once

#include <string>

/**
 * @brief Function Base
 *
 */
typedef struct FunctionInfoBase {
  std::string name;
  std::string return_type;
  std::string signature;
  bool is_noexcept = false;
  bool is_constexpr = false;
  bool is_consteval = false;
} FunctionBase;

/**
 * @brief Member Function Info
 *
 */
typedef struct MemberFunctionInfo {
  FunctionInfoBase base;
  std::string class_name;
  bool is_const = false;
  bool is_polymorphic = false;
} MemberFunctionInfo;

/**
 * @brief Function Info
 *
 */
typedef struct FunctionInfo {
  FunctionInfoBase base;
  std::string namespace_name;
  bool is_extern = false;
  bool is_static = false;
} FunctionInfo;
