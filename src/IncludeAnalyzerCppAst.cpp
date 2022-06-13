#include "IncludeAnalyzerCppAst.h"

#include <algorithm>
#include <cppast/cpp_preprocessor.hpp>

namespace {
cppast::detail::visitor_filter_t IncludeWhiteList() {
  return cppast::whitelist<cppast::cpp_entity_kind::include_directive_t>();
}

}  // namespace

IncludeAnalyzerCppAst::IncludeAnalyzerCppAst(std::shared_ptr<CodeParserCppAst> p_parser) : p_parser_(p_parser) {}

IncludeAnalyzerCppAst::~IncludeAnalyzerCppAst() = default;

std::vector<IncludeInfo> IncludeAnalyzerCppAst::GetIncludes() noexcept {
  std::vector<IncludeInfo> infos;
  for (const auto &file : p_parser_->GetParserRef().files()) {
    cppast::visit(file, IncludeWhiteList(), [&](const cppast::cpp_entity &e, cppast::visitor_info info) {
      if (info.event == cppast::visitor_info::container_entity_exit) return true;
      if (e.kind() == cppast::cpp_include_directive::kind()) {
        IncludeInfo info = {};
        const auto &include = reinterpret_cast<const cppast::cpp_include_directive &>(e);

        p_parser_->Log("Include directive in:", include.target().name(), cppast::severity::debug);

        // kind
        info.kind = include.include_kind() == cppast::cpp_include_kind::system ? IncludeInfo::Kind::kSystem
                                                                               : IncludeInfo::Kind::kLocal;
        // name
        info.name = include.target().name();
        // name
        info.full_path = include.full_path();

        p_parser_->Log("Include directive out:", include.target().name(), cppast::severity::debug);

        infos.push_back(info);
      }
      return true;
    });
  }
  return infos;
}
