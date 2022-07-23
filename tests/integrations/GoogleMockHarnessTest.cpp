#include <gtest/gtest.h>

#include "GoogleMockHarness.h"
#include "gtgen.h"

const std::string kSourceTreePath = SOURCE_DIR;

TEST(GoogleMockHarnessIntegration, Create) {
  std::vector<std::string> paths = {
      kSourceTreePath + "tests/integrations/testdata/Function.h",
  };
  std::string compile_database = "./";
  GoogleMockHarness harness("", gtgen::CreateAnalyzer(paths, compile_database, true));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Generate() << std::endl;
}

// TEST(TestTargetScopeRelationViewerHarness, Create) {
//   std::vector<std::string> paths = {
//       kSourceTreePath + "include/AbstractTestHarness.h",
//       kSourceTreePath + "include/GoogleMockHarness.h",
//       kSourceTreePath + "include/CodeAnalyzerInterface.h",
//       kSourceTreePath + "tests/integrations/testdata/Function.h",
//       kSourceTreePath + "tests/integrations/testdata/MemberFunction.h",
//       kSourceTreePath + "tests/integrations/testdata/MockGenerate.h",
//   };
//   std::string compile_database = "./";
//   TestTargetScopeRelationViewerHarness harness(gtgen::CreateAnalyzer(paths, compile_database, true));
//   EXPECT_TRUE(harness.Ready());
//   std::cout << harness.Generate() << std::endl;
// }
//