#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct FunctionInfoBase {
  std::string name;
  std::string return_type;
  std::string signature;
  std::vector<std::string> parameters;
  std::vector<std::string> scope;
  bool is_noexcept = false;
  bool is_constexpr = false;
  bool is_consteval = false;
  bool is_variadic = false;
  bool is_deleted = false;

  /**
   * @brief operator== for overload check
   *
   * @param rhs rgiht hand side
   * @return true
   * @return false
   *
   * @details see https://stackoverflow.com/questions/9568852/overloading-by-return-type
   */
  bool operator==(const FunctionInfoBase &rhs) const noexcept {
    return (name == rhs.name && parameters == rhs.parameters && scope == rhs.scope);
  }

  bool operator!=(const FunctionInfoBase &rhs) const noexcept {
    return !(this->operator==(rhs));
  }

  bool InScopeOf(const std::vector<std::string> &rhs_scope) const noexcept {
    return scope.size() >= rhs_scope.size() && std::equal(rhs_scope.begin(), rhs_scope.end(), scope.begin());
  }
} FunctionBase;

/**
 * @brief Member Function Info
 *
 */
typedef struct MemberFunctionInfo {
  enum class AccessSpecifier : int { kPublic, kPrivate, kProtected };
  FunctionInfoBase base;
  std::string class_name;
  enum AccessSpecifier access_specifier;
  bool is_const = false;
  bool is_volatile = false;
  bool is_polymorphic = false;  /// virtual, override or final
  std::vector<std::string> base_classes;

  bool operator==(const MemberFunctionInfo &rhs) const noexcept {
    return (base == rhs.base && is_const == rhs.is_const && is_volatile == rhs.is_volatile);
  }

  bool operator!=(const MemberFunctionInfo &rhs) const noexcept {
    return !(this->operator==(rhs));
  }

  bool InScopeOf(const std::vector<std::string> &rhs_scope) const noexcept {
    return base.InScopeOf(rhs_scope);
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

  bool operator==(const FunctionInfo &rhs) const noexcept {
    return (base == rhs.base);
  }

  bool operator!=(const FunctionInfo &rhs) const noexcept {
    return !(this->operator==(rhs));
  }

  bool InScopeOf(const std::vector<std::string> &rhs_scope) const noexcept {
    return base.InScopeOf(rhs_scope);
  }
} FunctionInfo;

typedef struct FunctionScope {
  std::vector<std::string> scope_names;

  FunctionScope(const std::vector<std::string> rhs_scope_names) : scope_names(rhs_scope_names) {
  }

  bool operator==(const FunctionScope &rhs) const noexcept {
    return scope_names == rhs.scope_names;
  }

  bool In(const FunctionScope &rhs) const noexcept {
    return scope_names.size() >= rhs.scope_names.size() &&
           std::equal(rhs.scope_names.begin(), rhs.scope_names.end(), scope_names.begin());
  }

  std::string FullName() const noexcept {
    if (scope_names.empty()) return "";
    std::string full_name;
    for (const auto &it : scope_names)
      full_name += it + "::";
    return full_name.substr(0, full_name.size() - 2);
  }
} FunctionScope;

class FunctionAttributeInterface {
 public:
  FunctionAttributeInterface() = default;
  virtual ~FunctionAttributeInterface() = default;

  virtual bool operator==(const FunctionAttributeInterface &rhs) const noexcept = 0;
  virtual bool operator!=(const FunctionAttributeInterface &rhs) const noexcept = 0;

  virtual bool InScopeOf(const FunctionScope &rhs) const noexcept = 0;

  /**
   * @brief Constant expression
   *
   * @return std::string "constexpr" "consteval" or ""
   */
  virtual std::string ConstantExpression() const noexcept = 0;  // 5.19 Constant expressions
  /**
   * @brief Definition suffix
   *
   * @return std::string "deleted" "default" or ""
   */
  virtual std::string DefinitionSuffix() const noexcept = 0;  // 8.4.3 Deleted definitions

  /**
   * @brief Exception suffix
   *
   * @return std::string  "noexcept" or ""
   */
  virtual std::string ExceptionSuffix() const noexcept = 0;  // 15.4 Exception specification
  virtual FunctionScope Scope() const noexcept = 0;
  virtual std::string Name() const noexcept = 0;
  virtual std::string ReturnType() const noexcept = 0;
  virtual std::string ParameterList() const noexcept = 0;
  virtual std::vector<std::string> Parameters() const noexcept = 0;

  virtual bool IsClassMember() const noexcept = 0;  // 9.2 Class members

  /**
   * @brief AccessSpecifier
   *
   * @return std::string "public" "private" "protected" or ""
   */
  virtual std::string AccessSpecifier() const noexcept = 0;  // 9.2 Class members

  /**
   * @brief CvQualifier
   *
   * @return std::string "const" "volatile" "const volatile" or ""
   */
  virtual std::string CvQualifier() const noexcept = 0;  // 9.3 cv-qualifiers

  /**
   * @brief polymorphic availability
   *
   * @return true
   * @return false
   * @details If function is virtual override or final, it returns true
   */
  virtual bool IsAbleToPolymorphic() const noexcept = 0;

  virtual std::vector<std::string> BaseClasses() const noexcept = 0;

  virtual std::string StorageClass() const noexcept = 0;  // At most one (7.1.1 Storage class specifiers)
};

class FunctionAttributeBase : public FunctionAttributeInterface {
 public:
  virtual ~FunctionAttributeBase() override;

  bool operator==(const FunctionAttributeInterface &rhs) const noexcept final;
  bool operator!=(const FunctionAttributeInterface &rhs) const noexcept final;

  bool InScopeOf(const FunctionScope &rhs) const noexcept final;

  std::string ConstantExpression() const noexcept final;  // 5.19 Constant expressions
  std::string DefinitionSuffix() const noexcept final;    // 8.4.3 Deleted definitions
  std::string ExceptionSuffix() const noexcept final;     // 15.4 Exception specification
  FunctionScope Scope() const noexcept final;
  std::string Name() const noexcept final;
  std::string ReturnType() const noexcept final;
  std::string ParameterList() const noexcept final;
  std::vector<std::string> Parameters() const noexcept final;

 protected:
  FunctionAttributeBase(const FunctionInfoBase &info);

 private:
  FunctionInfoBase info_;
};

class MemberFunctionAttribute : public FunctionAttributeBase {
 public:
  explicit MemberFunctionAttribute(const MemberFunctionInfo &info);
  virtual ~MemberFunctionAttribute() override;

  bool IsClassMember() const noexcept final;           // 9.2 Class members
  std::string AccessSpecifier() const noexcept final;  // 9.2 Class members
  std::string CvQualifier() const noexcept final;      // 9.3 cv-qualifiers
  bool IsAbleToPolymorphic() const noexcept final;
  std::vector<std::string> BaseClasses() const noexcept final;

  std::string StorageClass() const noexcept final;  // At most one (7.1.1 Storage class specifiers)

  MemberFunctionInfo GetAttribute();

 private:
  MemberFunctionInfo info_;
};

class NamespaceFunctionAttribute : public FunctionAttributeBase {
 public:
  explicit NamespaceFunctionAttribute(const FunctionInfo &info);
  virtual ~NamespaceFunctionAttribute() override;

  bool IsClassMember() const noexcept final;           // 9.2 Class members
  std::string AccessSpecifier() const noexcept final;  // 9.2 Class members
  std::string CvQualifier() const noexcept final;      // 9.3 cv-qualifiers
  bool IsAbleToPolymorphic() const noexcept final;
  std::vector<std::string> BaseClasses() const noexcept final;

  std::string StorageClass() const noexcept final;  // At most one (7.1.1 Storage class specifiers)

  FunctionInfo GetAttribute();

 private:
  FunctionInfo info_;
};
