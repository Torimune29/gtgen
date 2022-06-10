#include <gtest/gtest.h>
#include "FunctionParser.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

const std::string kSourceTreePath = SOURCE_DIR;

TEST(FunctionParserTest, GetPaths_Settings_NotReady) {
  std::vector<std::string> paths_expected = {"test", "test1", "test2"};
  std::string compile_database_expected = "foo";
  std::unordered_map<std::string, std::string> settings_expected = {{"verbose", "false"}, {"compile_database_path", compile_database_expected}};
  FunctionParser parser(paths_expected, compile_database_expected, false);

  auto paths = parser.GetFilePaths();
  auto settings = parser.GetSettings();
  EXPECT_EQ(paths, paths_expected);
  EXPECT_EQ(settings, settings_expected);
  EXPECT_FALSE(parser.Ready());
}

TEST(FunctionParserTest, Ready) {
  std::vector<std::string> paths = {
    kSourceTreePath + "src/AbstractCodeParser.cpp",
    kSourceTreePath + "src/CodeParserCppAst.cpp",
    kSourceTreePath + "src/FunctionParser.cpp",
    kSourceTreePath + "tests/testdata/Function.h",
    kSourceTreePath + "tests/testdata/MemberFunction.h",
  };
  std::string compile_database = "./";
  FunctionParser parser(paths, compile_database, false);

  EXPECT_TRUE(parser.Ready());
}

TEST(FunctionParserTest, ValidateFunctionInfo) {
  std::vector<std::string> paths = {
    kSourceTreePath + "tests/testdata/Function.h",
  };
  std::string compile_database = "./";
  FunctionParser parser(paths, compile_database, false);

  EXPECT_TRUE(parser.Ready());
  for (const auto &it : parser.GetFunction()) {
    if (it.base.name == "normal") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "noexcept_only") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_TRUE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "noexcept_false") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "noexcept_complex") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);  // except without noexcept or noexcept(true)
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "extern_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_TRUE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "static_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_TRUE(it.is_static);
    }
    if (it.base.name == "constexpr_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_TRUE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "static_constexpr") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_TRUE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_TRUE(it.is_static);
    }
    if (it.base.name == "namespace_deleted") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_TRUE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "namespace_normal") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
    if (it.base.name == "namespace_type_return") {
      EXPECT_STREQ(it.base.return_type.c_str(), "ns::m");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_FALSE(it.is_extern);
      EXPECT_FALSE(it.is_static);
    }
  }
}

TEST(FunctionParserTest, ValidateMemberFunctionInfo) {
  std::vector<std::string> paths = {
    kSourceTreePath + "tests/testdata/MemberFunction.h",
  };
  std::string compile_database = "./";
  FunctionParser parser(paths, compile_database, false);

  EXPECT_TRUE(parser.Ready());
  for (const auto &it : parser.GetMemberFunction()) {
    if (it.base.name == "const_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_TRUE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
    }
    if (it.base.name == "volatile_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_TRUE(it.is_volatile);
    }
    if (it.base.name == "full_suffix") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_TRUE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_TRUE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_TRUE(it.is_volatile);
    }
    if (it.base.name == "noise_suffix") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_TRUE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
    }
    if (it.base.name == "virtual_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      {
        SCOPED_TRACE( ::testing::Message() << it.class_name );
        EXPECT_TRUE(it.class_name == "foo"
          || it.class_name == "bar");
      }
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_FALSE(it.is_const);
      EXPECT_TRUE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
      if (it.class_name == "foo") {
        EXPECT_EQ(it.base_classes, std::vector<std::string>());
      } else if (it.class_name == "bar") {
        EXPECT_EQ(it.base_classes, std::vector<std::string>({"foo<int>"}));
      } else {
        EXPECT_TRUE(false);
      }
    }
    if (it.base.name == "pure_virtual") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      {
        SCOPED_TRACE( ::testing::Message() << it.class_name );
        EXPECT_TRUE(it.class_name == "foo"
          || it.class_name == "bar");
      }
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_TRUE(it.is_const);
      EXPECT_TRUE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
      if (it.class_name == "foo") {
        EXPECT_EQ(it.base_classes, std::vector<std::string>());
      } else if (it.class_name == "bar") {
        EXPECT_EQ(it.base_classes, std::vector<std::string>({"foo<int>"}));
      } else {
        EXPECT_TRUE(false);
      }
    }
    if (it.base.name == "with_definition") {
      static bool duplicated = false;
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_TRUE(it.class_name == "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
      EXPECT_FALSE(duplicated);  // check unique (declaration or definition)
      duplicated = true;
    }
    if (it.base.name == "deleted") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_TRUE(it.base.is_deleted);
      EXPECT_TRUE(it.class_name == "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
    }
    if (it.base.name == "struct_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_STREQ(it.class_name.c_str(), "baz");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
    }
    if (it.base.name == "private_function" || it.base.name == "private_function_2") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPrivate);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
    }
    if (it.base.name == "protected_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kProtected);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
    }
    if (it.base.name == "public_function") {
      EXPECT_STREQ(it.base.return_type.c_str(), "void");
      EXPECT_STREQ(it.base.signature.c_str(), "()");
      EXPECT_TRUE(it.base.parameters.empty());
      EXPECT_FALSE(it.base.is_noexcept);
      EXPECT_FALSE(it.base.is_constexpr);
      EXPECT_FALSE(it.base.is_consteval);
      EXPECT_FALSE(it.base.is_variadic);
      EXPECT_FALSE(it.base.is_deleted);
      EXPECT_STREQ(it.class_name.c_str(), "foo");
      EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
      EXPECT_FALSE(it.is_const);
      EXPECT_FALSE(it.is_polymorphic);
      EXPECT_FALSE(it.is_volatile);
    }
    if (it.base.name == "overload") {
      if (it.base.parameters.at(0) == "int") {
        static size_t count = 0;
        EXPECT_STREQ(it.base.return_type.c_str(), "int");
        EXPECT_STREQ(it.base.signature.c_str(), "(int)");
        EXPECT_EQ(it.base.parameters.size(), 1);
        EXPECT_FALSE(it.base.is_noexcept);
        EXPECT_FALSE(it.base.is_constexpr);
        EXPECT_FALSE(it.base.is_consteval);
        EXPECT_FALSE(it.base.is_variadic);
        EXPECT_FALSE(it.base.is_deleted);
        EXPECT_STREQ(it.class_name.c_str(), "foo");
        EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
        if (count == 0) {
          EXPECT_FALSE(it.is_const);
          EXPECT_FALSE(it.is_volatile);
        } else if (count == 1) {
          EXPECT_TRUE(it.is_const);
          EXPECT_FALSE(it.is_volatile);
        } else if (count == 2) {
          EXPECT_FALSE(it.is_const);
          EXPECT_TRUE(it.is_volatile);
        } else {
          EXPECT_TRUE(it.is_const);
          EXPECT_TRUE(it.is_volatile);
        }
        EXPECT_FALSE(it.is_polymorphic);
        count++;
      } else {
        EXPECT_STREQ(it.base.return_type.c_str(), "int");
        EXPECT_STREQ(it.base.signature.c_str(), "(double)");
        EXPECT_EQ(it.base.parameters.size(), 1);
        EXPECT_FALSE(it.base.is_noexcept);
        EXPECT_FALSE(it.base.is_constexpr);
        EXPECT_FALSE(it.base.is_consteval);
        EXPECT_FALSE(it.base.is_variadic);
        EXPECT_FALSE(it.base.is_deleted);
        EXPECT_STREQ(it.class_name.c_str(), "foo");
        EXPECT_EQ(it.access_specifier, MemberFunctionInfo::AccessSpecifier::kPublic);
        EXPECT_FALSE(it.is_const);
        EXPECT_FALSE(it.is_polymorphic);
        EXPECT_FALSE(it.is_volatile);
      }
    }
  }
}
