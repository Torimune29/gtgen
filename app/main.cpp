// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#include <cstdlib>
#include <iostream>
#include <string>

#include "CLI/CLI.hpp"
#include "FunctionParserCppAst.h"
#include "exampleConfig.h"
#include "jsoncons/json.hpp"

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main(int argc, char *argv[]) {
  std::cout << "gtgen version: " << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << "."
            << PROJECT_VERSION_PATCH << "." << PROJECT_VERSION_TWEAK << std::endl;

  CLI::App app{"gtget"};
  std::string file = "";
  std::string compile_database = "./";
  app.add_option("-f,--file", file, "Path");
  app.add_option("-p,--compile-database", compile_database, "Compile Database Path(like compile_commands.json)");
  CLI11_PARSE(app, argc, argv)

  FunctionParserCppAst parser({file}, compile_database);

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
  for (const auto &it : parser.GetMemberFunctionInfos()) {
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
  for (const auto &it : parser.GetFunctionInfos()) {
    jsoncons::json each_functions(jsoncons::json_object_arg, {
                                                                 {"functionName", it.base.name},
                                                                 {"signature", it.base.signature},
                                                                 {"returnType", it.base.return_type},
                                                                 {"noexcept", it.base.is_noexcept},
                                                                 {"constexpr", it.base.is_constexpr},
                                                                 {"consteval", it.base.is_consteval},
                                                                 {"namespace", it.namespace_name},
                                                                 {"extern", it.is_extern},
                                                                 {"static", it.is_static},
                                                             });
    functions.push_back(std::move(each_functions));
  }
  result["function"] = std::move(functions);
  result["memberFunction"] = std::move(member_functions);
  std::cout << jsoncons::pretty_print(result) << std::endl;
  return return_code;
}
