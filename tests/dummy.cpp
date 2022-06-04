#include <gtest/gtest.h>
#include "AbstractCodeParser.h"
#include "CodeParserCppAst.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

TEST(AbstractCodeParserTest, NotReady) {
  std::vector<std::string> paths_expected = {"test", "test1", "test2"};
  AbstractCodeParser parser(paths_expected);
  auto paths = parser.GetFilePaths();
  auto settings = parser.GetSettings();
  EXPECT_EQ(paths, paths_expected);
  EXPECT_TRUE(settings.empty());
  EXPECT_FALSE(parser.Ready());
}

TEST(CodeParserCppAstTest, GetPaths_Settings_NotReady) {
  std::vector<std::string> paths_expected = {"test", "test1", "test2"};
  std::string compile_database_expected = "foo";
  std::unordered_map<std::string, std::string> settings_expected = {{"compile_database_path", compile_database_expected}};
  CodeParserCppAst parser(paths_expected, compile_database_expected);

  auto paths = parser.GetFilePaths();
  auto settings = parser.GetSettings();
  EXPECT_EQ(paths, paths_expected);
  EXPECT_EQ(settings, settings_expected);
  EXPECT_FALSE(parser.Ready());
}

TEST(CodeParserCppAstTest, Ready) {
  std::vector<std::string> paths= {"../src/AbstractCodeParser.cpp"};
  std::string compile_database = "./";
  CodeParserCppAst parser(paths, compile_database);

  EXPECT_TRUE(parser.Ready());
}
