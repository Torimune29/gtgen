// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#include <cstdlib>

#include <iostream>
#include <string>

#include "CLI/CLI.hpp"
#include "CodeParserCppAst.h"
#include "exampleConfig.h"

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main(int argc, char *argv[]) {
  std::cout << "gtget version: " << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << "."
            << PROJECT_VERSION_PATCH << "." << PROJECT_VERSION_TWEAK << std::endl;

  CLI::App app{"gtget"};
  std::string file = "src/AbstcractCodeParser.cpp";
  std::string compile_database = "compile_commands.json";
  app.add_option("-f,--file", file, "Path");
  app.add_option("-p,--compile-database", compile_database, "Compile Database Path(like compile_commands.json)");
  CLI11_PARSE(app, argc, argv)

  CodeParserCppAst parser({file}, compile_database);

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
  return return_code;
}
