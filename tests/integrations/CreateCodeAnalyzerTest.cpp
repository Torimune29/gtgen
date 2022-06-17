#include <gtest/gtest.h>

#include "gtgen.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

const std::string kSourceTreePath = SOURCE_DIR;

TEST(CreateCodeAnalyzerTest, GetPaths_Settings_NotReady) {
  std::vector<std::string> paths_expected = {"test", "test1", "test2"};
  std::string compile_database_expected = "foo";
  std::unordered_map<std::string, std::string> settings_expected = {
      {"verbose", "false"}, {"compile_database_path", compile_database_expected}};
  auto analyzer = gtgen::CreateAnalyzer(paths_expected, compile_database_expected, false);

  EXPECT_FALSE(analyzer != nullptr);
}

TEST(CreateCodeAnalyzerTest, Ready) {
  std::vector<std::string> paths = {
      kSourceTreePath + "tests/integrations/testdata/Function.h",
      kSourceTreePath + "tests/integrations/testdata/MemberFunction.h",
  };
  std::string compile_database = "./";
  auto analyzer = gtgen::CreateAnalyzer(paths, compile_database, false);

  EXPECT_TRUE(analyzer != nullptr);
}
