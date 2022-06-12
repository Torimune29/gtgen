#include "FunctionParser.h"

#include "FunctionParserImpl.h"

FunctionParser::FunctionParser(const std::vector<std::string> &file_paths, const std::string &compile_database_path,
                               bool verbose)
    : p_impl_(new FunctionParserImpl(file_paths, compile_database_path, verbose)) {
  file_paths_ = p_impl_->GetFilePaths();
  settings_ = p_impl_->GetSettings();
}

FunctionParser::~FunctionParser() = default;

bool FunctionParser::Ready() noexcept { return p_impl_->Ready(); }

std::vector<std::shared_ptr<FunctionAttributeInterface>> FunctionParser::Get() noexcept { return p_impl_->Get(); }

