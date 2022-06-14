#include <gtest/gtest.h>

#include "GoogleMockHarness.h"
#include "gtgen.h"
#include "mock/mock_CodeAnalyzerInterface.h"

using ::testing::Return;

const std::string kSourceTreePath = SOURCE_DIR;

TEST(GoogleMockHarness, Create) {
  std::vector<std::string> paths = {
      kSourceTreePath + "include/AbstractTestHarness.h",   kSourceTreePath + "include/GoogleMockHarness.h",
      kSourceTreePath + "include/CodeAnalyzerInterface.h", kSourceTreePath + "tests/testdata/Function.h",
      kSourceTreePath + "tests/testdata/MemberFunction.h", kSourceTreePath + "tests/testdata/MockGenerate.h",
  };
  std::string compile_database = "./";
  GoogleMockHarness harness("Test", gtgen::CreateAnalyzer(paths, compile_database, true));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Generate() << std::endl;
}

TEST(GoogleMockHarness, CreateWithMock) {
  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions()).WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>()));
  EXPECT_CALL(*p_analyzer, GetScopes()).WillOnce(Return(std::vector<ScopeInfo>()));
  EXPECT_CALL(*p_analyzer, GetIncludes()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Generate() << std::endl;
}
