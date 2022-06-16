#include "TestTargetViewerHarness.h"

#include <jsoncons_ext/jsonpath/jsonpath.hpp>

#include "jsoncons/json.hpp"

namespace {
jsoncons::ojson SetScopesRecursively(const ScopeInfo &info) {
  jsoncons::ojson scope_relations(jsoncons::json_object_arg, {
                                                                 {"name", info.name},
                                                                 {"kind", static_cast<int>(info.kind)},
                                                                 {"fullScope", info.full_scope},
                                                             });
  jsoncons::ojson children(jsoncons::json_array_arg);
  for (const auto &it : info.children) {
    children.push_back(SetScopesRecursively(it));
  }
  scope_relations["children"] = children;
  return scope_relations;
}

jsoncons::ojson SetFunctionBase(const std::shared_ptr<FunctionAttributeInterface> &p_if) {
  return jsoncons::ojson(jsoncons::json_object_arg, {
                                                        {"functionName", p_if->Name()},
                                                        {"signature", p_if->Signature()},
                                                        {"parameterTypes", p_if->ParameterTypes()},
                                                        {"parameters", p_if->Parameters()},
                                                        {"declaration", p_if->Declaration()},
                                                        {"scope", p_if->Scope().scope_names},
                                                        {"returnType", p_if->ReturnType()},
                                                        {"constantExpression", p_if->ConstantExpression()},
                                                        {"exceptionSuffix", p_if->ExceptionSuffix()},
                                                        {"definitionSuffix", p_if->DefinitionSuffix()},
                                                    });
}

jsoncons::ojson SetIncludesRecursively(const IncludeInfo &info) {
  jsoncons::ojson includes(jsoncons::json_object_arg, {
                                                          {"name", info.name},
                                                          {"kind", static_cast<int>(info.kind)},
                                                          {"fullPath", info.full_path},
                                                      });
  return includes;
}

void PrettyPrint(const jsoncons::ojson &json) {
  jsoncons::json_options options;
  options.escape_all_non_ascii(true);
  json.dump_pretty(std::cout, options);
}

}  // namespace

bool TestTargetFunctionViewerHarness::Ready() noexcept {
  jsoncons::ojson result(jsoncons::json_object_arg, {{"notice", notice_message_}, {"function", ""}});
  jsoncons::ojson functions(jsoncons::json_array_arg);

  auto v_func = p_analyzer_->GetFunctions();
  for (const auto &it : v_func) {
    jsoncons::ojson each_functions(jsoncons::json_object_arg,
                                   {
                                       {"base", SetFunctionBase(it)},
                                       {"storageClass", it->StorageClass()},
                                       {"isClassMember", it->IsClassMember()},
                                       {"accessSpecifier", it->AccessSpecifier()},
                                       {"constMemberFunction", it->CvQualifier()},
                                       {"polymorphicMemberFunction", it->IsAbleToPolymorphic()},
                                       {"baseClasses", it->BaseClasses()},
                                   });
    functions.push_back(std::move(each_functions));
  }
  result["function"] = std::move(functions);
  PrettyPrint(result);

  return true;
}

bool TestTargetScopeRelationViewerHarness::Ready() noexcept {
  jsoncons::ojson result(jsoncons::json_object_arg, {{"notice", notice_message_}, {"scopeRelations", ""}});
  jsoncons::ojson scopes(jsoncons::json_array_arg);

  auto v_scopes = p_analyzer_->GetScopes();
  for (const auto &it : v_scopes) {
    scopes.push_back(SetScopesRecursively(it));
  }
  result["scopeRelations"] = std::move(scopes);
  PrettyPrint(result);
  return true;
}

bool TestTargetLocalIncludeViewerHarness::Ready() noexcept {
  jsoncons::ojson result(jsoncons::json_object_arg, {{"notice", notice_message_}, {"includes", ""}});
  jsoncons::ojson includes(jsoncons::json_array_arg);

  auto v_includes = p_analyzer_->GetIncludes();
  for (const auto &it : v_includes) {
    includes.push_back(SetIncludesRecursively(it));
  }
  result["includes"] = std::move(includes);
  PrettyPrint(result);
  return true;
}
