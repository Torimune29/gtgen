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

TEST(GoogleMockHarness, FreeFunctions) {
  FunctionInfo info_1 = {};
  info_1.base.name = "test1";
  info_1.base.return_type = "void";
  info_1.base.signature = "(int,uint32_t*)";
  info_1.base.parameters = {"int", "uint32_t*"};
  info_1.base.scope = {""};
  info_1.base.is_constexpr = false;
  info_1.base.is_consteval = false;
  info_1.base.is_noexcept = false;
  info_1.base.is_deleted = false;
  info_1.is_extern = false;
  info_1.is_static = false;
  const std::string mock_label = "Test";
  const std::string mock_method_1 = "MOCK_METHOD2(test1, void(int,uint32_t*));";
  const std::string free_function_class = mock_label + "FreeFunction";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  std::shared_ptr<FunctionAttributeInterface> p_function_if(new NamespaceFunctionAttribute(info_1));
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes()).WillOnce(Return(std::vector<ScopeInfo>()));
  EXPECT_CALL(*p_analyzer, GetIncludes()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  EXPECT_TRUE(body.find(mock_label) != std::string::npos);
  EXPECT_TRUE(body.find(mock_method_1) != std::string::npos);
}
