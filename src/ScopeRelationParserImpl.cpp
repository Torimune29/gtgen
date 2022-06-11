#include "ScopeRelationParserImpl.h"

#include <algorithm>
#include <cppast/cpp_class.hpp>
#include <cppast/cpp_namespace.hpp>
#include <iostream>
#include <type_safe/optional_ref.hpp>

namespace {
cppast::detail::visitor_filter_t ScopeRelationWhiteList() {
  return cppast::whitelist<cppast::cpp_entity_kind::namespace_t, cppast::cpp_entity_kind::class_t>();
}

}  // namespace

ScopeRelationParserImpl::ScopeRelationParserImpl(const std::vector<std::string> &file_paths,
                                       const std::string &compile_database_path, bool verbose)
    : CodeParserCppAst(std::move(file_paths), ScopeRelationWhiteList(), std::move(compile_database_path), verbose) {}

ScopeRelationParserImpl::~ScopeRelationParserImpl() = default;

std::vector<ScopeInfo> ScopeRelationParserImpl::Get() noexcept {
  if (!ready_) return {};

  std::vector<ScopeInfo> infos;
  for (const auto &file : p_parser_->files()) {
    cppast::visit(file, filter_, [&](const cppast::cpp_entity &e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      // type handling
      std::vector<ScopeInfo> infos_parsed;
      if (e.kind() == cppast::cpp_namespace::kind()) {
        Log("Scope Global Namespace in:", e.name(), cppast::severity::debug);
        infos_parsed = ParseNamespaceScopeRelation(e);
        Log("Scope Global Namespace out:", e.name(), cppast::severity::debug);
      } else if (e.kind() == cppast::cpp_class::kind()) {
        Log("Scope Global Class in:", e.name(), cppast::severity::debug);
        infos_parsed = ParseClassScopeRelation(e, true);
        Log("Scope Global Class out:", e.name(), cppast::severity::debug);
      }
      infos.insert(infos.end(), infos_parsed.begin(), infos_parsed.end());
      return true;
    });
  }
  return infos;
}

template <typename T>
std::vector<ScopeInfo> ScopeRelationParserImpl::ParseScopeRelation(const T &entity) const noexcept {
  std::vector<ScopeInfo> infos, infos_parsed;
  if (entity.kind() == cppast::cpp_namespace::kind()) {
    Log("Scope Namespace in:", entity.name(), cppast::severity::debug);
    infos_parsed = ParseNamespaceScopeRelation(entity);
    Log("Scope Namespace out:", entity.name(), cppast::severity::debug);
  } else if (entity.kind() == cppast::cpp_class::kind()) {
    Log("Scope Class in:", entity.name(), cppast::severity::debug);
    infos_parsed = ParseClassScopeRelation(entity, false);
    Log("Scope Class out:", entity.name(), cppast::severity::debug);
  }
  infos.insert(infos.end(), infos_parsed.begin(), infos_parsed.end());
  return infos;
}


template <typename T>
std::vector<ScopeInfo> ScopeRelationParserImpl::ParseNamespaceScopeRelation(const T &entity) const noexcept {
  std::vector<ScopeInfo> infos;
  if (entity.kind() == cppast::cpp_namespace::kind()) {
    const auto &namespace_e = reinterpret_cast<const cppast::cpp_namespace &>(entity);
    ScopeInfo info;
    // name
    info.name = namespace_e.name();
    // kind
    info.kind = namespace_e.is_anonymous() ? ScopeInfo::Kind::kAnonymousNamespace : ScopeInfo::Kind::kNamespace;
    // full name
    auto full_scope = GetScopes(namespace_e);
    if (!full_scope.empty())
      info.full_scope = full_scope;
    // children
    for (auto &child : namespace_e) {
      auto info_parsed = ParseScopeRelation(child);
      info.children.insert(info.children.end(), info_parsed.begin(), info_parsed.end());
    }
    infos.push_back(info);
  }
  return infos;
}

template <typename T>
std::vector<ScopeInfo> ScopeRelationParserImpl::ParseClassScopeRelation(const T &entity, bool expect_global) const noexcept {
  std::vector<ScopeInfo> infos;
  if (entity.kind() == cppast::cpp_class::kind()) {
    const auto &class_e = reinterpret_cast<const cppast::cpp_class &>(entity);
    // scoped or global class check
    if (expect_global && !GetFullName(class_e).empty()) {
      Log("Scope Global Class skip:", class_e.name(), cppast::severity::debug);
      return {};
    }
    ScopeInfo info;
    // name
    info.name = class_e.name();
    // kind
    info.kind = ScopeInfo::Kind::kClass;
    // full name
    if (expect_global) {
      info.full_scope = std::vector<std::string>({class_e.name()});
    } else {
      auto full_scope = GetScopes(class_e);
      if (!full_scope.empty())
        info.full_scope = full_scope;
      if (!class_e.name().empty()) info.full_scope.push_back(class_e.name());
      Log("Scope Class name:", class_e.name(), cppast::severity::debug);
    }
    // children. According to C++ standard, namespace as class children is not be able to exist.
    for (auto &child : class_e) {
      auto info_parsed = ParseClassScopeRelation(child, false);
      info.children.insert(info.children.end(), info_parsed.begin(), info_parsed.end());
    }
    infos.push_back(info);
  }
  return infos;
}

