#include <gtest/gtest.h>
#include "TestTargetViewerHarness.h"


const std::string kSourceTreePath = SOURCE_DIR;

TEST(TestTargetFunctionViewerHarness, Create) {
  std::vector<std::string> paths = {
    kSourceTreePath + "include/AbstractCodeParser.h",
    kSourceTreePath + "include/AbstractTestHarness.h",
    kSourceTreePath + "include/TestTargetViewerHarness.h",
    kSourceTreePath + "include/FunctionParser.h",
    kSourceTreePath + "tests/testdata/Function.h",
    kSourceTreePath + "tests/testdata/MemberFunction.h",
    kSourceTreePath + "tests/testdata/Scope.h",
  };
  std::string compile_database = "./";
  TestTargetFunctionViewerHarness harness(std::shared_ptr<FunctionParser>(new FunctionParser(paths, compile_database, true)));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Generate() << std::endl;
}

TEST(TestTargetScopeRelationViewerHarness, Create) {
  std::vector<std::string> paths = {
    kSourceTreePath + "include/AbstractCodeParser.h",
    kSourceTreePath + "include/AbstractTestHarness.h",
    kSourceTreePath + "include/TestTargetViewerHarness.h",
    kSourceTreePath + "include/FunctionParser.h",
    kSourceTreePath + "tests/testdata/Function.h",
    kSourceTreePath + "tests/testdata/MemberFunction.h",
    kSourceTreePath + "tests/testdata/Scope.h",
  };
  std::string compile_database = "./";
  TestTargetScopeRelationViewerHarness harness(std::shared_ptr<ScopeRelationParser>(new ScopeRelationParser(paths, compile_database, true)));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Generate() << std::endl;
}
