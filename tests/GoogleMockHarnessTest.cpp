#include <gtest/gtest.h>
#include "GoogleMockHarness.h"


const std::string kSourceTreePath = SOURCE_DIR;

TEST(GoogleMockHarness, Create) {
  std::vector<std::string> paths = {
    kSourceTreePath + "include/AbstractCodeParser.h",
    kSourceTreePath + "include/AbstractTestHarness.h",
    kSourceTreePath + "include/GoogleMockHarness.h",
    kSourceTreePath + "include/FunctionParser.h",
    kSourceTreePath + "tests/testdata/Function.h",
    kSourceTreePath + "tests/testdata/MemberFunction.h",
  };
  std::string compile_database = "./";
  GoogleMockHarness harness("Test", std::shared_ptr<FunctionParser>(new FunctionParser(paths, compile_database, false)));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Create() << std::endl;
}
