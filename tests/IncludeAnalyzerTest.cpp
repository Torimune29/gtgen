#include <gtest/gtest.h>
#include <string>

#include "IncludeInfo.h"
#include "gtgen.h"
#include "TestTargetViewerHarness.h"


const std::string kSourceTreePath = SOURCE_DIR;


TEST(IncludeAnalyzerTest, ValidateIncludeInfo) {
  std::vector<std::string> paths = {
      kSourceTreePath + "./include/CodeAnalyzerInterface.h",
  };
  std::string compile_database = "./";
  auto parser = gtgen::CreateAnalyzer(paths, compile_database, true);

  auto result = parser->GetIncludes();
  EXPECT_EQ(result.size(), 6);
  std::string base_path = std::string(kSourceTreePath);
  base_path = base_path.substr(0, base_path.size() - 3);  // exclude last ../
  for (const auto &it : result) {
    SCOPED_TRACE(::testing::Message() << it.name);
    SCOPED_TRACE(::testing::Message() << static_cast<int>(it.kind));
    SCOPED_TRACE(::testing::Message() << it.full_path);
    if (it.name == "string") {
      EXPECT_EQ(it.kind, IncludeInfo::Kind::kSystem);
      EXPECT_TRUE(it.full_path.find("string") != std::string::npos);
    }
    if (it.name == "vector") {
      EXPECT_EQ(it.kind, IncludeInfo::Kind::kSystem);
      EXPECT_TRUE(it.full_path.find("vector") != std::string::npos);
    }
    if (it.name == "unordered_map") {
      EXPECT_EQ(it.kind, IncludeInfo::Kind::kSystem);
      EXPECT_TRUE(it.full_path.find("unordered_map") != std::string::npos);
    }
    if (it.name == "IncludeInfo.h") {
      EXPECT_EQ(it.kind, IncludeInfo::Kind::kLocal);
      EXPECT_TRUE(it.full_path.find("include/IncludeInfo.h") != std::string::npos);
    }
    if (it.name == "ScopeInfo.h") {
      EXPECT_EQ(it.kind, IncludeInfo::Kind::kLocal);
      EXPECT_TRUE(it.full_path.find("include/ScopeInfo.h") != std::string::npos);
    }
    if (it.name == "FunctionInfo.h") {
      EXPECT_EQ(it.kind, IncludeInfo::Kind::kLocal);
      EXPECT_TRUE(it.full_path.find("include/FunctionInfo.h") != std::string::npos);
    }
  }
}

TEST(IncludeAnalyzerTest, Chain) {
  std::vector<std::string> paths = {
      kSourceTreePath + "./include/CodeAnalyzerInterface.h",
  };
  std::string compile_database = "./";
  auto parser = gtgen::CreateAnalyzer(paths, compile_database, true);

  auto result = parser->GetIncludes();
  std::vector<std::string> paths_include_parent;
  for (const auto &it : result) {
    paths_include_parent.push_back(it.full_path);
  }
  TestTargetScopeRelationViewerHarness harness(gtgen::CreateAnalyzer(paths_include_parent, compile_database, true));
  harness.Ready();
  std::cout << harness.Generate() << std::endl;
}
