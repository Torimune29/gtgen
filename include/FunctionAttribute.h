#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct FunctionScope {
  std::vector<std::string> scope_names;

  explicit FunctionScope(const std::vector<std::string> rhs_scope_names) : scope_names(rhs_scope_names) {
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
  virtual std::string Signature() const noexcept = 0;
  /**
   * @brief ParameterTypes
   *
   * @return std::vector<std::string> Formal parameter types
   * @details
   * Formal parameter type lists.
   */
  virtual std::vector<std::string> ParameterTypes() const noexcept = 0;
  /**
   * @brief Parameters
   *
   * @return std::vector<std::pair<std::string, std::string>>
   * @details
   * Parameters. First is formal, and second is actual.
   */
  virtual std::vector<std::pair<std::string, std::string>> Parameters() const noexcept = 0;
  virtual bool IsOverloadedOperator() const noexcept = 0;
  virtual std::string Declaration() const noexcept = 0;

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
