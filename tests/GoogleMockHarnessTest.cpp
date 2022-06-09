#include <gtest/gtest.h>
#include "GoogleMockHarness.h"


const std::string kSourceTreePath = SOURCE_DIR;

TEST(GoogleMockHarness, Create) {
  std::vector<std::string> paths = {
    kSourceTreePath + "tests/testdata/Function.h",
  };
  std::string compile_database = "./";
  GoogleMockHarness harness("Test", std::shared_ptr<FunctionParser>(new FunctionParser(paths, compile_database, false)));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Create() << std::endl;
}
