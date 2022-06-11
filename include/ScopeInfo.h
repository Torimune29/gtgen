#pragma once

#include <string>
#include <vector>

typedef struct ScopeInfo {
  enum class Kind : int {
    kGlobal = 0,
    kNamespace,
    kAnonymousNamespace,
    kClass,
  };
  std::string name;
  Kind kind;
  std::vector<std::string> full_scope;
  std::vector<ScopeInfo> children;

  bool operator== (const ScopeInfo &rhs) const noexcept {
    return (name == rhs.name
      && kind == rhs.kind
      && children == rhs.children
    );
  }
} ScopeInfo;
