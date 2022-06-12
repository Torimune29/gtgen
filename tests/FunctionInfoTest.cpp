#include <gtest/gtest.h>
#include <algorithm>

#include "FunctionParser.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

const std::string kSourceTreePath = SOURCE_DIR;


TEST(FunctionInfoTest, EqualOperator) {
  MemberFunctionInfo func_1 = {}, func_2 = {};
  func_1.base.scope = {"fizz", "buzz"};
  func_2.base.scope = {"n3", "fizz", "buzz"};
  EXPECT_NE(func_1, func_2);

  // filter
  MemberFunctionInfo func_3 = {};
    func_3.base.scope = {"foo"};
    func_3.base.name = "overload";
    func_3.base.parameters.push_back("int");
    func_3.is_const = true;
    func_3.is_volatile = true;
  MemberFunctionInfo filter_match = {};
  filter_match.base.name = "overload";
  filter_match.base.parameters.push_back("int");
  filter_match.base.scope.push_back("foo");
  filter_match.is_const = true;
  filter_match.is_volatile = true;
  EXPECT_EQ(func_3, filter_match);
  MemberFunctionInfo filter_1 = filter_match,
    filter_2 = filter_match,
    filter_3 = filter_match,
    filter_4 = filter_match,
    filter_5 = filter_match;
  {
    filter_1.base.name = "override";
    EXPECT_NE(func_3, filter_1);
  }
  {
    filter_2.base.parameters.push_back("float");
    EXPECT_NE(func_3, filter_2);
  }
  {
    filter_3.base.scope.at(0) = "bar";
    EXPECT_NE(func_3, filter_3);
  }
  {
    filter_4.is_const = false;
    EXPECT_NE(func_3, filter_4);
  }
  {
    filter_5.is_volatile = false;
    EXPECT_NE(func_3, filter_5);
  }
}

TEST(FunctionInfoTest, EqualOperatorFreeFunction) {
  {
    FunctionInfo func_3 = {};
    func_3.base.name = "static_function";
    func_3.is_static = true;
    func_3.is_extern = false;

    // filter
    FunctionInfo filter_match = {};
    filter_match.base.name = "static_function";
    EXPECT_EQ(func_3, filter_match);
    FunctionInfo filter_1 = filter_match,
      filter_2 = filter_match,
      filter_3 = filter_match;
    {
      filter_1.base.name = "extern_function";
      EXPECT_NE(func_3, filter_1);
    }
    {
      filter_2.base.parameters.push_back("float");
      EXPECT_NE(func_3, filter_2);
    }
    {
      filter_3.base.scope.push_back("bar");
      EXPECT_NE(func_3, filter_3);
    }
  }
}


TEST(FunctionInfoTest, InScopeOf) {
  FunctionInfo func_1 = {};
  MemberFunctionInfo func_2 = {};
  func_1.base.scope = {"fizz", "buzz"};
  func_2.base.scope = {"n3", "fizz", "buzz"};

  EXPECT_TRUE(func_1.InScopeOf({"fizz"}));
  EXPECT_FALSE(func_1.InScopeOf({"buzz"}));
  EXPECT_TRUE(func_1.InScopeOf({}));  // empty as global
  EXPECT_TRUE(func_1.InScopeOf({"fizz", "buzz"}));

  EXPECT_FALSE(func_2.InScopeOf({"fizz"}));
  EXPECT_FALSE(func_2.InScopeOf({"buzz"}));
  EXPECT_FALSE(func_2.InScopeOf({"n3", "buzz"}));
  EXPECT_FALSE(func_2.InScopeOf({"fizz", "buzz"}));
  EXPECT_TRUE(func_2.InScopeOf({}));  // empty as global
  EXPECT_TRUE(func_2.InScopeOf({"n3"}));
  EXPECT_TRUE(func_2.InScopeOf({"n3", "fizz"}));
  EXPECT_TRUE(func_2.InScopeOf({"n3", "fizz", "buzz"}));
}
