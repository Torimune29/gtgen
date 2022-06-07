// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#include <cstdlib>
#include <iostream>
#include <string>

#include "CLI/CLI.hpp"
#include "FunctionParser.h"
#include "ProjectVersion.h"
#include "jsoncons/json.hpp"
#include <jsoncons_ext/jsonpath/jsonpath.hpp>


/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main(int argc, char *argv[]) {
  std::cout << "gtgen version: " << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << "."
            << PROJECT_VERSION_PATCH << std::endl;

  CLI::App app{"gtget"};
  std::vector<std::string> files = {""};
  std::string compile_database = "./";
  bool verbose_parse = false;
  std::vector<std::string> filter = {};
  app.add_option("-f,--files", files, "Analyze file paths.")->required();
  app.add_option("-p,--compile-database", compile_database, "Compile database directory path")->required();
  app.add_flag("--verbose-parse", verbose_parse, "Verbose parse result");
  app.add_option("--easy-filter", filter, "Parse result filter. e.g. memberFunction returnType \\\"void\\\"")->expected(3);
  CLI11_PARSE(app, argc, argv)

  FunctionParser parser(files, compile_database, verbose_parse);

  std::cout << "file: ";
  for (const auto &it : parser.GetFilePaths()) {
    std::cout << it << ",";
  }
  std::cout << std::endl;

  std::cout << "settings: " << std::endl;
  for (const auto &it : parser.GetSettings()) {
    std::cout << "  " << it.first << ": " << it.second << std::endl;
  }
  auto ready = parser.Ready();
  int return_code = 1;
  if (ready) {
    return_code = 0;
  }
  jsoncons::json result(jsoncons::json_object_arg, {{"function", ""}, {"memberFunction", ""}});
  jsoncons::json functions(jsoncons::json_array_arg), member_functions(jsoncons::json_array_arg);
  for (const auto &it : parser.GetMemberFunction()) {
    jsoncons::json each_functions(jsoncons::json_object_arg,
                                  {
                                      {"functionName", it.base.name},
                                      {"signature", it.base.signature},
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
  // jsoncons::json each_member_functions(jsoncons::json_array_arg);
  for (const auto &it : parser.GetFunction()) {
    jsoncons::json each_functions(jsoncons::json_object_arg, {
                                                                 {"functionName", it.base.name},
                                                                 {"signature", it.base.signature},
                                                                 {"returnType", it.base.return_type},
                                                                 {"noexcept", it.base.is_noexcept},
                                                                 {"constexpr", it.base.is_constexpr},
                                                                 {"consteval", it.base.is_consteval},
                                                                 {"extern", it.is_extern},
                                                                 {"static", it.is_static},
                                                             });
    functions.push_back(std::move(each_functions));
  }
  result["function"] = std::move(functions);
  result["memberFunction"] = std::move(member_functions);
  if (filter.empty()) {
    std::cout << jsoncons::pretty_print(result) << std::endl;
  } else {
    std::string query = "$." + filter[0] + "[?(@." + filter[1] + " == " + filter[2] + ")]";
    auto result_filtered = jsoncons::jsonpath::json_query(result, query);
    std::cout << jsoncons::pretty_print(result_filtered) << std::endl;
  }
  return return_code;
}
