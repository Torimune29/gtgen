#include <gtest/gtest.h>

#include "ScopeRelationParser.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

const std::string kSourceTreePath = SOURCE_DIR;


TEST(ScopeRelationParserTest, ValidateScopeInfo) {
  std::vector<std::string> paths = {
      kSourceTreePath + "tests/testdata/Scope.h",
  };
  std::string compile_database = "./";
  ScopeRelationParser parser(paths, compile_database, true);

  EXPECT_TRUE(parser.Ready());
  auto result = parser.Get();
  EXPECT_EQ(result.size(), 1);
  for (const auto &it : result) {
    SCOPED_TRACE(::testing::Message() << it.name);
    SCOPED_TRACE(::testing::Message() << static_cast<int>(it.kind));
    SCOPED_TRACE(::testing::Message() << it.children.size());
    if (it.name == "n1") {
      EXPECT_EQ(it.kind, ScopeInfo::Kind::kNamespace);
      EXPECT_EQ(it.children.size(), 2);
      for (const auto &it_child : it.children) {
        if (it_child.name == "I3"
          || it_child.name == "I4") {
          EXPECT_EQ(it_child.kind, ScopeInfo::Kind::kClass);
          EXPECT_EQ(it_child.children.size(), 0);
        }
      }
    }
  }
}
