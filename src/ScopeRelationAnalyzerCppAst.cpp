#include "ScopeRelationAnalyzerCppAst.h"

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

ScopeRelationAnalyzerCppAst::ScopeRelationAnalyzerCppAst(std::shared_ptr<CodeParserCppAst> p_parser)
    : p_parser_(p_parser) {
}

ScopeRelationAnalyzerCppAst::~ScopeRelationAnalyzerCppAst() = default;


std::vector<ScopeInfo> ScopeRelationAnalyzerCppAst::GetScopes() noexcept {
  std::vector<ScopeInfo> infos;
  for (const auto &file : p_parser_->GetParserRef().files()) {
    cppast::visit(file, ScopeRelationWhiteList(), [&](const cppast::cpp_entity &e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      // type handling
      std::vector<ScopeInfo> infos_parsed;
      if (e.kind() == cppast::cpp_namespace::kind()) {
        p_parser_->Log("Scope Global Namespace in:", e.name(), cppast::severity::debug);
        infos_parsed = ParseNamespaceScopeRelation(e);
        p_parser_->Log("Scope Global Namespace out:", e.name(), cppast::severity::debug);
      } else if (e.kind() == cppast::cpp_class::kind()) {
        p_parser_->Log("Scope Global Class in:", e.name(), cppast::severity::debug);
        infos_parsed = ParseClassScopeRelation(e, true);
        p_parser_->Log("Scope Global Class out:", e.name(), cppast::severity::debug);
      }
      infos.insert(infos.end(), infos_parsed.begin(), infos_parsed.end());
      return true;
    });
  }
  return infos;
}


template <typename T>
std::vector<ScopeInfo> ScopeRelationAnalyzerCppAst::ParseScopeRelation(const T &entity) const noexcept {
  std::vector<ScopeInfo> infos, infos_parsed;
  if (entity.kind() == cppast::cpp_namespace::kind()) {
    p_parser_->Log("Scope Namespace in:", entity.name(), cppast::severity::debug);
    infos_parsed = ParseNamespaceScopeRelation(entity);
    p_parser_->Log("Scope Namespace out:", entity.name(), cppast::severity::debug);
  } else if (entity.kind() == cppast::cpp_class::kind()) {
    p_parser_->Log("Scope Class in:", entity.name(), cppast::severity::debug);
    infos_parsed = ParseClassScopeRelation(entity, false);
    p_parser_->Log("Scope Class out:", entity.name(), cppast::severity::debug);
  }
  infos.insert(infos.end(), infos_parsed.begin(), infos_parsed.end());
  return infos;
}


template <typename T>
std::vector<ScopeInfo> ScopeRelationAnalyzerCppAst::ParseNamespaceScopeRelation(const T &entity) const noexcept {
  std::vector<ScopeInfo> infos;
  if (entity.kind() == cppast::cpp_namespace::kind()) {
    const auto &namespace_e = reinterpret_cast<const cppast::cpp_namespace &>(entity);
    ScopeInfo info;
    // name
    info.name = namespace_e.name();
    // kind
    info.kind = namespace_e.is_anonymous() ? ScopeInfo::Kind::kAnonymousNamespace : ScopeInfo::Kind::kNamespace;
    // full name
    auto full_scope = p_parser_->GetScopes(namespace_e);
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
std::vector<ScopeInfo> ScopeRelationAnalyzerCppAst::ParseClassScopeRelation(const T &entity, bool expect_global) const noexcept {
  std::vector<ScopeInfo> infos;
  if (entity.kind() == cppast::cpp_class::kind()) {
    const auto &class_e = reinterpret_cast<const cppast::cpp_class &>(entity);
    // scoped or global class check
    if (expect_global && !p_parser_->GetScopes(class_e).empty()) {
      p_parser_->Log("Scope Global Class skip:", class_e.name(), cppast::severity::debug);
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
      info.full_scope = p_parser_->GetScopes(class_e);
      if (!class_e.name().empty()) info.full_scope.push_back(class_e.name());
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

