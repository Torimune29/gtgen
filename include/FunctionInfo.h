#pragma once

#include <string>

/**
 * @brief Function Base
 *
 */
typedef struct FunctionInfoBase {
  std::string name;
  std::string return_type;   // func.return_type()
  std::string signature;     // func.signature() ex: (int, float) <-  int b(int a, float* b = nullptr);  ex: () const <-
                             // void c() const;
  bool is_noexcept = false;  // func.noexcept_condition()
  bool is_constexpr = false;  // func.is_constxpr()
  bool is_consteval = false;  // func.is_consteval()
} FunctionBase;

/**
 * @brief Member Function Info
 *
 */
typedef struct MemberFunctionInfo {
  FunctionInfoBase base;
  std::string class_name;
  bool is_const = false;  // func.cv_qualifier() == cpp_cv_const || func.cv_qualifier() == cpp_cv_const_volatile
  bool is_polymorphic = false;
} MemberFunctionInfo;

/**
 * @brief Function Info
 *
 */
typedef struct FunctionInfo {
  FunctionInfoBase base;
  bool is_extern = false;  // func.storage_class() == cpp_storage_class_extern
  bool is_static = false;  // func.storage_class() == cpp_storage_class_static
} FunctionInfo;
