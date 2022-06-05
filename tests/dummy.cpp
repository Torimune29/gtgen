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

TEST(FunctionParserCppAstTest, ValidateFunctionInfo) {
  std::vector<std::string> paths = {
    kSourceTreePath + "tests/testdata/Function.h",
  };
  std::string compile_database = "./";
  FunctionParserCppAst parser(paths, compile_database);

  EXPECT_TRUE(parser.Ready());
  for (const auto &it : parser.GetFunctionInfos()) {
    if (it.base.name == "a") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "b") {
      EXPECT_STREQ(it.base.return_type.c_str(), "int");
      EXPECT_STREQ(it.base.signature.c_str(), "(int,float*)");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "c") {
      EXPECT_STREQ(it.base.return_type.c_str(), "int(&)[10]");
      EXPECT_STREQ(it.base.signature.c_str(), "(decltype(42),...)");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_TRUE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "d") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "e") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "f") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);  // except without noexcept or noexcept(true)
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "g") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_TRUE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "h") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_TRUE(it.is_static);
    }
    if (it.base.name == "i") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_TRUE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "j") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_TRUE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_TRUE(it.is_static);
    }
    if (it.base.name == "k") {  // delete unsupported
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "l") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      // definition contains namespace but declaration does not.
      // EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "m") {
      EXPECT_STREQ(it.base.return_type.c_str(), "ns::m");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "n") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "(int)");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.namespace_name.c_str(), "");
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
  }
}
