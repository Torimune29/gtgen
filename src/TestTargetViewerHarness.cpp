#include "TestTargetViewerHarness.h"

#include "jsoncons/json.hpp"
#include <jsoncons_ext/jsonpath/jsonpath.hpp>


bool TestTargetViewerHarness::Ready() noexcept {
  bool okay = p_parser_->Ready();
  if (okay) {
    jsoncons::json result(jsoncons::json_object_arg, {{"function", ""}, {"memberFunction", ""}});
    jsoncons::json functions(jsoncons::json_array_arg), member_functions(jsoncons::json_array_arg);

    auto v_func = p_parser_->GetFunction();
    for (const auto &it : v_func) {
      jsoncons::json each_functions(jsoncons::json_object_arg, {
                                                                   {"functionName", it.base.name},
                                                                   {"signature", it.base.signature},
                                                                   {"parameterNum", it.base.parameters.size()},
                                                                   {"returnType", it.base.return_type},
                                                                   {"noexcept", it.base.is_noexcept},
                                                                   {"constexpr", it.base.is_constexpr},
                                                                   {"consteval", it.base.is_consteval},
                                                                   {"extern", it.is_extern},
                                                                   {"static", it.is_static},
                                                               });
      functions.push_back(std::move(each_functions));
    }
    auto v_member_func = p_parser_->GetMemberFunction();
    for (const auto &it : v_member_func) {
      jsoncons::json each_functions(jsoncons::json_object_arg,
                                    {
                                        {"functionName", it.base.name},
                                        {"signature", it.base.signature},
                                        {"parameterNum", it.base.parameters.size()},
                                        {"returnType", it.base.return_type},
                                        {"noexcept", it.base.is_noexcept},
                                        {"constexpr", it.base.is_constexpr},
                                        {"consteval", it.base.is_consteval},
                                        {"className", it.class_name},
                                        {"accessSpecifier", static_cast<int>(it.access_specifier)},
                                        {"constMemberFunction", it.is_const},
                                        {"polymorphicMemberFunction", it.is_polymorphic},
                                    });
      member_functions.push_back(std::move(each_functions));
    }
    result["function"] = std::move(functions);
    result["memberFunction"] = std::move(member_functions);
    std::cout << jsoncons::pretty_print(result) << std::endl;
  }
  return okay;
}