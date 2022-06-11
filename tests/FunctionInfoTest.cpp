#include <gtest/gtest.h>
#include <algorithm>

#include "FunctionParser.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

const std::string kSourceTreePath = SOURCE_DIR;


TEST(FunctionInfoTest, EqualOperator) {
  std::vector<std::string> paths = {
      kSourceTreePath + "tests/testdata/MemberFunction.h",
  };
  std::string compile_database = "./";
  FunctionParser parser(paths, compile_database, true);

  EXPECT_TRUE(parser.Ready());
  auto result = parser.GetMemberFunction();
  auto it_func_1 = std::find_if(result.begin(), result.end(), [] (const MemberFunctionInfo &info) {
    return info.base.scope == std::vector<std::string>({"fizz", "buzz"}); });
  auto it_func_2 = std::find_if(result.begin(), result.end(), [] (const MemberFunctionInfo &info) {
    return info.base.scope == std::vector<std::string>({"n3", "fizz", "buzz"}); });
  EXPECT_TRUE(!(*it_func_1 == *it_func_2));
}

TEST(FunctionInfoTest, InScopeOf) {
  std::vector<std::string> paths = {
      kSourceTreePath + "tests/testdata/MemberFunction.h",
  };
  std::string compile_database = "./";
  FunctionParser parser(paths, compile_database, true);

  EXPECT_TRUE(parser.Ready());
  auto result = parser.GetMemberFunction();
  auto it_func_1 = std::find_if(result.begin(), result.end(), [] (const MemberFunctionInfo &info) {
    return info.base.scope == std::vector<std::string>({"fizz", "buzz"}); });
  auto it_func_2 = std::find_if(result.begin(), result.end(), [] (const MemberFunctionInfo &info) {
    return info.base.scope == std::vector<std::string>({"n3", "fizz", "buzz"}); });

  EXPECT_TRUE(it_func_1->InScopeOf({"fizz"}));
  EXPECT_FALSE(it_func_1->InScopeOf({"buzz"}));
  EXPECT_TRUE(it_func_1->InScopeOf({}));  // empty as global
  EXPECT_TRUE(it_func_1->InScopeOf({"fizz", "buzz"}));

  EXPECT_FALSE(it_func_2->InScopeOf({"fizz"}));
  EXPECT_FALSE(it_func_2->InScopeOf({"buzz"}));
  EXPECT_FALSE(it_func_2->InScopeOf({"n3", "buzz"}));
  EXPECT_FALSE(it_func_2->InScopeOf({"fizz", "buzz"}));
  EXPECT_TRUE(it_func_2->InScopeOf({}));  // empty as global
  EXPECT_TRUE(it_func_2->InScopeOf({"n3"}));
  EXPECT_TRUE(it_func_2->InScopeOf({"n3", "fizz"}));
  EXPECT_TRUE(it_func_2->InScopeOf({"n3", "fizz", "buzz"}));
}
