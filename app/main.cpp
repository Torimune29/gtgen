// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#include <cstdlib>
#include <iostream>
#include <string>

#include "CLI/CLI.hpp"
#include "gtgen.h"
#include "CodeAnalyzerInterface.h"
#include "GoogleMockHarness.h"
#include "TestTargetViewerHarness.h"
#include "ProjectInformation.h"


int main(int argc, char *argv[]) {
  CLI::App app{PROJECT_NAME_VERSION};
  std::vector<std::string> files = {""};
  std::string compile_database = "./";
  bool verbose = false;
  int mode = 0;
  std::string mock_label = "mock_";
  app.add_option("-f,--files", files, "Analyze file paths. If empty, parse all files using compile database.");
  app.add_option("-p,--compile-database", compile_database, "Compile database directory path")->required();
  app.add_flag("--verbose", verbose, "Verbose parse result");
  app.add_flag("--mock{0}, --view-functions{10}, --view-scopes{11}, --view-includes{12}", mode, "Mode");
  app.add_option("--mock-label", mock_label, "Mock Label for class. (--mock only)");
  CLI11_PARSE(app, argc, argv)

  auto analyzer = gtgen::CreateAnalyzer(files, compile_database, verbose);
  std::shared_ptr<AbstractTestHarness> p_harness;
  switch (mode) {
    case 0:
      p_harness = decltype(p_harness)(new GoogleMockHarness(mock_label, analyzer));
      break;
    case 10:
      p_harness = decltype(p_harness)(new TestTargetFunctionViewerHarness(analyzer));
      break;
    case 11:
      p_harness = decltype(p_harness)(new TestTargetScopeRelationViewerHarness(analyzer));
      break;
    case 12:
      p_harness = decltype(p_harness)(new TestTargetLocalIncludeViewerHarness(analyzer));
      break;
    default:
      return 1;
  }

  int return_code = 1;
  if (p_harness->Ready()) {
    std::cout << p_harness->Generate() << std::endl;
    return_code = 0;
  }
  return return_code;
}
