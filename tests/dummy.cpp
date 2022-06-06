#include <gtest/gtest.h>
#include "FunctionParserCppAst.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

const std::string kSourceTreePath = SOURCE_DIR;

TEST(FunctionParserCppAstTest, GetPaths_Settings_NotReady) {
  std::vector<std::string> paths_expected = {"test", "test1", "test2"};
  std::string compile_database_expected = "foo";
  std::unordered_map<std::string, std::string> settings_expected = {{"compile_database_path", compile_database_expected}};
  FunctionParserCppAst parser(paths_expected, compile_database_expected, false);

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
  FunctionParserCppAst parser(paths, compile_database, false);

  EXPECT_TRUE(parser.Ready());
  for (const auto &it : parser.GetMemberFunctionInfos()) {
    std::cout << "function: " << it.base.name
      << ", signature: " << it.base.signature
      << ", return: " << it.base.return_type
      << ", noexcept: " << it.base.is_noexcept
      << ", class: " << it.class_name
      << ", access_specifier: " << static_cast<int>(it.access_specifier)
      << ", const: " << it.is_const
      << ", polymorphic: " << it.is_polymorphic << std::endl;
  }
  for (const auto &it : parser.GetFunctionInfos()) {
    std::cout << "function: " << it.base.name
      << ", signature: " << it.base.signature
      << ", return: " << it.base.return_type
      << ", noexcept: " << it.base.is_noexcept
      << ", namespace: " << it.namespace_name
      << ", extern: " << it.is_extern
      << ", static: " << it.is_static << std::endl;
  }
}

TEST(FunctionParserCppAstTest, ValidateFunctionInfo) {
  std::vector<std::string> paths = {
    kSourceTreePath + "tests/testdata/Function.h",
  };
  std::string compile_database = "./";
  FunctionParserCppAst parser(paths, compile_database, false);

  EXPECT_TRUE(parser.Ready());
  for (const auto &it : parser.GetFunctionInfos()) {
    if (it.base.name == "normal") {
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
    if (it.base.name == "noexcept_only") {
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
    if (it.base.name == "noexcept_false") {
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
    if (it.base.name == "noexcept_complex") {
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
    if (it.base.name == "extern_function") {
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
    if (it.base.name == "static_function") {
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
    if (it.base.name == "constexpr_function") {
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
    if (it.base.name == "static_constexpr") {
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
    if (it.base.name == "namespace_normal") {
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
    if (it.base.name == "namespace_type_return") {
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
  }
}

TEST(FunctionParserCppAstTest, ValidateMemberFunctionInfo) {
  std::vector<std::string> paths = {
    kSourceTreePath + "tests/testdata/MemberFunction.h",
  };
  std::string compile_database = "./";
  FunctionParserCppAst parser(paths, compile_database, false);

  EXPECT_TRUE(parser.Ready());
  for (const auto &it : parser.GetMemberFunctionInfos()) {
    if (it.base.name == "const_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::Public);
      EXPECT_TRUE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
    }
    if (it.base.name == "full_suffix") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::Public);
      EXPECT_TRUE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
    }
    if (it.base.name == "noise_suffix") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::Public);
      EXPECT_TRUE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
    }
    if (it.base.name == "virtual_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      {
        SCOPED_TRACE( ::testing::Message() << it.class_name );
        EXPECT_TRUE(it.class_name == "foo"
          || it.class_name == "bar");
      }
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::Public);
      EXPECT_FALSE(it.is_const);
      EXPECT_TRUE(it.is_polymorphic);
    }
    if (it.base.name == "pure_virtual") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      {
        SCOPED_TRACE( ::testing::Message() << it.class_name );
        EXPECT_TRUE(it.class_name == "foo"
          || it.class_name == "bar");
      }
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::Public);
      EXPECT_TRUE(it.is_const);
      EXPECT_TRUE(it.is_polymorphic);
    }
    if (it.base.name == "struct_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.class_name.c_str(), "baz");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::Public);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
    }
    if (it.base.name == "private_function" || it.base.name == "private_function_2") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::Private);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
    }
    if (it.base.name == "protected_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::Protected);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
    }
    if (it.base.name == "public_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::Public);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
    }
  }
}
