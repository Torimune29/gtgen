// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#include <cstdlib>
#include <iostream>
#include <string>

#include "CLI/CLI.hpp"
#include "FunctionParser.h"
#include "GoogleMockHarness.h"
#include "TestTargetViewerHarness.h"
#include "ProjectInformation.h"


int main(int argc, char *argv[]) {
  CLI::App app{PROJECT_NAME_VERSION};
  std::vector<std::string> files = {""};
  std::string compile_database = "./";
  bool verbose = false;
  bool view_only = false;
  app.add_option("-f,--files", files, "Analyze file paths. If empty, parse all files using compile database.");
  app.add_option("-p,--compile-database", compile_database, "Compile database directory path")->required();
  app.add_flag("--verbose", verbose, "Verbose parse result");
  app.add_flag("--view-only", view_only, "View only");
  CLI11_PARSE(app, argc, argv)

  std::shared_ptr<FunctionParser> p_parser(new FunctionParser(files, compile_database, verbose));
  std::shared_ptr<AbstractTestHarness<FunctionParser>> p_harness;
  if (view_only) {
    p_harness = decltype(p_harness)(new TestTargetViewerHarness(p_parser));
  } else {
    p_harness = decltype(p_harness)(new GoogleMockHarness("Mock", p_parser));
  }

  int return_code = 1;
  if (p_harness->Ready()) {
    std::cout << p_harness->Generate() << std::endl;
    return_code = 0;
  }
  return return_code;
}
