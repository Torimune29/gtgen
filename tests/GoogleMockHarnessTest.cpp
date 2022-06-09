#include <gtest/gtest.h>
#include "GoogleMockHarness.h"


const std::string kSourceTreePath = SOURCE_DIR;

TEST(GoogleMockHarness, Create) {
  std::vector<std::string> paths = {
    kSourceTreePath + "build/_deps/CLI11-src/include/CLI/App.hpp",
    kSourceTreePath + "build/_deps/cppast-src/include/cppast/cpp_entity.hpp",
    kSourceTreePath + "include/FunctionParser.h",
    kSourceTreePath + "tests/testdata/Function.h",
    kSourceTreePath + "tests/testdata/MemberFunction.h",
  };
  std::string compile_database = "./";
  GoogleMockHarness harness("Test", std::shared_ptr<FunctionParser>(new FunctionParser(paths, compile_database, false)));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Create() << std::endl;
}
