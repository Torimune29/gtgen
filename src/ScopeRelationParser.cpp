#include "ScopeRelationParser.h"

#include "ScopeRelationParserImpl.h"

ScopeRelationParser::ScopeRelationParser(const std::vector<std::string> &file_paths, const std::string &compile_database_path,
                               bool verbose)
    : p_impl_(new ScopeRelationParserImpl(file_paths, compile_database_path, verbose)) {
  file_paths_ = p_impl_->GetFilePaths();
  settings_ = p_impl_->GetSettings();
}

ScopeRelationParser::~ScopeRelationParser() = default;

bool ScopeRelationParser::Ready() noexcept { return p_impl_->Ready(); }

std::vector<ScopeInfo> ScopeRelationParser::Get() noexcept { return p_impl_->Get(); }
