#include <gtest/gtest.h>
#include "TestTargetViewerHarness.h"
#include "gtgen.h"


const std::string kSourceTreePath = SOURCE_DIR;

TEST(TestTargetFunctionViewerHarness, Create) {
  std::vector<std::string> paths = {
    kSourceTreePath + "include/AbstractTestHarness.h",
    kSourceTreePath + "include/GoogleMockHarness.h",
    kSourceTreePath + "include/CodeAnalyzerInterface.h",
    kSourceTreePath + "tests/testdata/Function.h",
    kSourceTreePath + "tests/testdata/MemberFunction.h",
    kSourceTreePath + "tests/testdata/MockGenerate.h",
  };
  std::string compile_database = "./";
  TestTargetFunctionViewerHarness harness(
    gtgen::CreateAnalyzer(paths, compile_database, true));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Generate() << std::endl;
}

TEST(TestTargetScopeRelationViewerHarness, Create) {
  std::vector<std::string> paths = {
    kSourceTreePath + "include/AbstractTestHarness.h",
    kSourceTreePath + "include/GoogleMockHarness.h",
    kSourceTreePath + "include/CodeAnalyzerInterface.h",
    kSourceTreePath + "tests/testdata/Function.h",
    kSourceTreePath + "tests/testdata/MemberFunction.h",
    kSourceTreePath + "tests/testdata/MockGenerate.h",
  };
  std::string compile_database = "./";
  TestTargetScopeRelationViewerHarness harness(
    gtgen::CreateAnalyzer(paths, compile_database, true));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Generate() << std::endl;
}
