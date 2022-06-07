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
  bool is_variadic = false;

  bool operator== (const FunctionInfoBase &rhs) const {
    return (name == rhs.name
      && return_type == rhs.return_type
      && signature == rhs.signature
      && is_noexcept == rhs.is_noexcept
      && is_constexpr == rhs.is_constexpr
      && is_consteval == rhs.is_consteval
      && is_variadic == rhs.is_variadic
    );
  }
} FunctionBase;

/**
 * @brief Member Function Info
 *
 */
typedef struct MemberFunctionInfo {
  enum class AccessSpecifier : int {
    kPublic,
    kPrivate,
    kProtected
  };
  FunctionInfoBase base;
  std::string class_name;
  enum AccessSpecifier access_specifier;
  bool is_const = false;
  bool is_polymorphic = false;

  bool operator== (const MemberFunctionInfo &rhs) const {
    return (base == rhs.base
      && class_name == rhs.class_name
      && access_specifier == rhs.access_specifier
      && is_const == rhs.is_const
      && is_polymorphic == rhs.is_polymorphic
    );
  }

} MemberFunctionInfo;

/**
 * @brief Function Info
 *
 */
typedef struct FunctionInfo {
  FunctionInfoBase base;
  bool is_extern = false;
  bool is_static = false;

  bool operator== (const FunctionInfo &rhs) const {
    return (base == rhs.base
      && is_extern == rhs.is_extern
      && is_static == rhs.is_static
    );
  }
} FunctionInfo;
