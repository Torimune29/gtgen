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
  EXPECT_NE(*it_func_1, *it_func_2);

  // filter
  auto it_func_3 = std::find_if(result.begin(), result.end(), [] (const MemberFunctionInfo &info) {
    return info.base.scope == std::vector<std::string>({"foo"})
    && info.base.name == "overload"
    && info.is_const && info.is_volatile; });
  MemberFunctionInfo filter_match = {};
  filter_match.base.name = "overload";
  filter_match.base.parameters.push_back("int");
  filter_match.base.scope.push_back("foo");
  filter_match.is_const = true;
  filter_match.is_volatile = true;
  EXPECT_EQ(*it_func_3, filter_match);
  MemberFunctionInfo filter_1 = filter_match,
    filter_2 = filter_match,
    filter_3 = filter_match,
    filter_4 = filter_match,
    filter_5 = filter_match;
  {
    filter_1.base.name = "override";
    EXPECT_NE(*it_func_3, filter_1);
  }
  {
    filter_2.base.parameters.push_back("float");
    EXPECT_NE(*it_func_3, filter_2);
  }
  {
    filter_3.base.scope.at(0) = "bar";
    EXPECT_NE(*it_func_3, filter_3);
  }
  {
    filter_4.is_const = false;
    EXPECT_NE(*it_func_3, filter_4);
  }
  {
    filter_5.is_volatile = false;
    EXPECT_NE(*it_func_3, filter_5);
  }
}

TEST(FunctionInfoTest, EqualOperatorFreeFunction) {
  std::vector<std::string> paths = {
      kSourceTreePath + "tests/testdata/Function.h",
  };
  std::string compile_database = "./";
  FunctionParser parser(paths, compile_database, true);

  EXPECT_TRUE(parser.Ready());
  auto result = parser.GetFunction();

  {
    // filter
    auto it_func_3 = std::find_if(result.begin(), result.end(), [] (const FunctionInfo &info) {
      return info.base.scope == std::vector<std::string>({})
      && info.base.name == "static_function"; });
    FunctionInfo filter_match = {};
    filter_match.base.name = "static_function";
    EXPECT_EQ(*it_func_3, filter_match);
    FunctionInfo filter_1 = filter_match,
      filter_2 = filter_match,
      filter_3 = filter_match;
    {
      filter_1.base.name = "extern_function";
      EXPECT_NE(*it_func_3, filter_1);
    }
    {
      filter_2.base.parameters.push_back("float");
      EXPECT_NE(*it_func_3, filter_2);
    }
    {
      filter_3.base.scope.push_back("bar");
      EXPECT_NE(*it_func_3, filter_3);
    }
  }

  {
    // filter on namespace
    auto it_func_3 = std::find_if(result.begin(), result.end(), [] (const FunctionInfo &info) {
      return info.base.scope == std::vector<std::string>({"ns"})
      && info.base.name == "namespace_normal"; });
    FunctionInfo filter_match = {};
    filter_match.base.name = "namespace_normal";
    filter_match.base.scope.push_back("ns");
    EXPECT_EQ(*it_func_3, filter_match);
    FunctionInfo filter_1 = filter_match,
      filter_2 = filter_match,
      filter_3 = filter_match;
    {
      filter_1.base.name = "extern_function";
      EXPECT_NE(*it_func_3, filter_1);
    }
    {
      filter_2.base.parameters.push_back("float");
      EXPECT_NE(*it_func_3, filter_2);
    }
    {
      filter_3.base.scope.push_back("bar");
      EXPECT_NE(*it_func_3, filter_3);
    }
  }
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

TEST(FunctionInfoTest, InScopeOfFreeFunction) {
  std::vector<std::string> paths = {
      kSourceTreePath + "tests/testdata/Function.h",
  };
  std::string compile_database = "./";
  FunctionParser parser(paths, compile_database, true);

  EXPECT_TRUE(parser.Ready());
  auto result = parser.GetFunction();
  auto it_func_1 = std::find_if(result.begin(), result.end(), [] (const FunctionInfo &info) {
    return info.base.name == "normal"; });

  EXPECT_FALSE(it_func_1->InScopeOf({"buzz"}));
  EXPECT_TRUE(it_func_1->InScopeOf({}));  // empty as global
}
