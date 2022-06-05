#include <gtest/gtest.h>
#include "FunctionParserCppAst.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

const std::string kSourceTreePath = SOURCE_DIR;

TEST(FunctionParserCppAstTest, GetPaths_Settings_NotReady) {
  std::vector<std::string> paths_expected = {"test", "test1", "test2"};
  std::string compile_database_expected = "foo";
  std::unordered_map<std::string, std::string> settings_expected = {{"compile_database_path", compile_database_expected}};
  FunctionParserCppAst parser(paths_expected, compile_database_expected);

  auto paths = parser.GetFilePaths();
  auto settings = parser.GetSettings();
  EXPECT_EQ(paths, paths_expected);
  EXPECT_EQ(settings, settings_expected);
  EXPECT_FALSE(parser.Ready());
}

TEST(FunctionParserCppAstTest, Ready) {
  std::vector<std::string> paths = {
    kSourceTreePath + "src/AbstractCodeParser.cpp",
    kSourceTreePath + "src/CodeParserCppAst.cpp",
    kSourceTreePath + "src/FunctionParserCppAst.cpp",
    kSourceTreePath + "tests/testdata/Function.h",
    kSourceTreePath + "tests/testdata/MemberFunction.h",
  };
  std::string compile_database = "./";
  FunctionParserCppAst parser(paths, compile_database);

  EXPECT_TRUE(parser.Ready());
  for (const auto &it : parser.GetMemberFunctionInfos()) {
    std::cout << "function: " << it.base.name
      << ", signature: " << it.base.signature
      << ", return: " << it.base.return_type
      << ", noexcept: " << it.base.is_noexcept
      << ", class: " << it.class_name
      << ", const: " << it.is_const
      << ", polymorphic: " << it.is_polymorphic << std::endl;
  }
  for (const auto &it : parser.GetFunctionInfos()) {
    std::cout << "function: " << it.base.name
      << ", signature: " << it.base.signature
      << ", return: " << it.base.return_type
      << ", noexcept: " << it.base.is_noexcept
      << ", namespace: " << it.namespace_name
      << ", const: " << it.is_extern
      << ", polymorphic: " << it.is_static << std::endl;
  }
}
