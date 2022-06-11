#include "TestTargetViewerHarness.h"

#include "jsoncons/json.hpp"
#include <jsoncons_ext/jsonpath/jsonpath.hpp>


namespace {
jsoncons::ojson SetScopesRecursively(const ScopeInfo &info) {
  jsoncons::ojson scope_relations(jsoncons::json_object_arg, {
                                                               {"name", info.name},
                                                               {"kind", static_cast<int>(info.kind)},
                                                               {"fullName", info.full_name},
                                                           });
  jsoncons::ojson children(jsoncons::json_array_arg);
  for (const auto &it : info.children) {
    children.push_back(SetScopesRecursively(it));
  }
  scope_relations["children"] = children;
  return scope_relations;
}
};  // namespace


bool TestTargetFunctionViewerHarness::Ready() noexcept {
  bool okay = p_parser_->Ready();
  if (okay) {
    jsoncons::ojson result(jsoncons::json_object_arg, {{"notice", notice_message_}, {"function", ""}, {"memberFunction", ""}});
    jsoncons::ojson functions(jsoncons::json_array_arg), member_functions(jsoncons::json_array_arg);

    auto v_func = p_parser_->GetFunction();
    for (const auto &it : v_func) {
      jsoncons::ojson each_functions(jsoncons::json_object_arg, {
                                                                   {"functionName", it.base.name},
                                                                   {"signature", it.base.signature},
                                                                   {"parameterNum", it.base.parameters.size()},
                                                                   {"scopes", it.base.scopes},
                                                                   {"returnType", it.base.return_type},
                                                                   {"noexcept", it.base.is_noexcept},
                                                                   {"constexpr", it.base.is_constexpr},
                                                                   {"consteval", it.base.is_consteval},
                                                                   {"deleted", it.base.is_deleted},
                                                                   {"extern", it.is_extern},
                                                                   {"static", it.is_static},
                                                               });
      functions.push_back(std::move(each_functions));
    }
    auto v_member_func = p_parser_->GetMemberFunction();
    for (const auto &it : v_member_func) {
      jsoncons::ojson each_functions(jsoncons::json_object_arg,
                                    {
                                        {"functionName", it.base.name},
                                        {"signature", it.base.signature},
                                        {"parameterNum", it.base.parameters.size()},
                                        {"scopes", it.base.scopes},
                                        {"returnType", it.base.return_type},
                                        {"noexcept", it.base.is_noexcept},
                                        {"constexpr", it.base.is_constexpr},
                                        {"consteval", it.base.is_consteval},
                                        {"deleted", it.base.is_deleted},
                                        {"className", it.class_name},
                                        {"accessSpecifier", static_cast<int>(it.access_specifier)},
                                        {"constMemberFunction", it.is_const},
                                        {"volatile", it.is_volatile},
                                        {"polymorphicMemberFunction", it.is_polymorphic},
                                        {"baseClasses", it.base_classes},
                                    });
      member_functions.push_back(std::move(each_functions));
    }
    result["function"] = std::move(functions);
    result["memberFunction"] = std::move(member_functions);
    std::cout << jsoncons::pretty_print(result) << std::endl;
  }
  return okay;
}

bool TestTargetScopeRelationViewerHarness::Ready() noexcept {
  bool okay = p_parser_->Ready();
  if (okay) {
    jsoncons::ojson result(jsoncons::json_object_arg, {{"notice", notice_message_}, {"scopeRelations", ""}});
    jsoncons::ojson scopes(jsoncons::json_array_arg);

    auto v_scopes = p_parser_->Get();
    for (const auto &it : v_scopes) {
      scopes.push_back(SetScopesRecursively(it));
    }
    result["scopeRelations"] = std::move(scopes);
    std::cout << jsoncons::pretty_print(result) << std::endl;
  }
  return okay;
}


