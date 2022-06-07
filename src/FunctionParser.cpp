#include "FunctionParser.h"

#include "FunctionParserImpl.h"

FunctionParser::FunctionParser(const std::vector<std::string> &file_paths, const std::string &compile_database_path,
                               bool verbose)
    : p_impl_(new FunctionParserImpl(file_paths, compile_database_path, verbose)) {}

FunctionParser::FunctionParser(const std::string &class_name, const std::vector<std::string> &file_paths,
                               const std::string &compile_database_path, bool verbose)
    : p_impl_(new FunctionParserImpl(class_name, file_paths, compile_database_path, verbose)) {}

FunctionParser::~FunctionParser() = default;

bool FunctionParser::Ready() { return p_impl_->Ready(); }

std::vector<FunctionInfo> FunctionParser::GetFunctionInfos() { return p_impl_->GetFunctionInfos(); }

std::vector<MemberFunctionInfo> FunctionParser::GetMemberFunctionInfos() { return p_impl_->GetMemberFunctionInfos(); }
