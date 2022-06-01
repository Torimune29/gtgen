#include <gtest/gtest.h>
#include "AbstractCodeParser.h"
#include "CodeParserCppAst.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

TEST(AbstractCodeParserTest, GetPath) {
  AbstractCodeParser parser("test", "compile_commands.json");
  EXPECT_STREQ(parser.GetFilePath().c_str(), "test");
  EXPECT_STREQ(parser.GetCompileDatabasePath().c_str(), "compile_commands.json");
}

TEST(CodeParserCppAstTest, GetPath) {
  CodeParserCppAst parser("test", "./");
  EXPECT_STREQ(parser.GetFilePath().c_str(), "test");
  EXPECT_STREQ(parser.GetCompileDatabasePath().c_str(), "./");
  EXPECT_TRUE(parser.Ready());
}
