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
  const std::string free_function_class = "class " + mock_label + "FreeFunction";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  std::shared_ptr<FunctionAttributeInterface> p_function_if(new NamespaceFunctionAttribute(info_1));
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes()).WillOnce(Return(std::vector<ScopeInfo>()));
  EXPECT_CALL(*p_analyzer, GetIncludes()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) != std::string::npos);
  EXPECT_TRUE(body.find(free_function_class) != std::string::npos);
}

TEST(GoogleMockHarness, ClassMemberFunctionsConst) {
  MemberFunctionInfo info_1 = {};
  info_1.base.name = "test1";
  info_1.base.return_type = "void";
  info_1.base.signature = "(int,uint32_t*)";
  info_1.base.parameters = {"int", "uint32_t*"};
  info_1.base.scope = {"TestClass1"};
  info_1.base.is_constexpr = false;
  info_1.base.is_consteval = false;
  info_1.base.is_noexcept = false;
  info_1.base.is_deleted = false;
  info_1.access_specifier = MemberFunctionInfo::AccessSpecifier::kPublic;
  info_1.class_name = "TestClass1";
  info_1.is_const = true;  // MOCK_CONST_METHOD
  info_1.is_volatile = false;
  info_1.is_polymorphic = false;

  ScopeInfo info_2 = {};
  info_2.name = "TestClass1";
  info_2.kind = ScopeInfo::Kind::kClass;
  info_2.full_scope = {"TestClass1"};
  info_2.children = {};

  const std::string mock_label = "Test";
  const std::string mock_method_1 = "MOCK_CONST_METHOD2(test1, void(int,uint32_t*));";
  const std::string function_class = "class " + mock_label + info_1.class_name;

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  std::shared_ptr<FunctionAttributeInterface> p_function_if(new MemberFunctionAttribute(info_1));
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes()).WillOnce(Return(std::vector<ScopeInfo>({info_2})));
  EXPECT_CALL(*p_analyzer, GetIncludes()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) != std::string::npos);
  EXPECT_TRUE(body.find(function_class) != std::string::npos);
}

/**
 * @brief Construct a new TEST object
 * @warning function in namespace scoped is unsupported
 *
 */
TEST(GoogleMockHarness, NamespaceFunctions) {
  FunctionInfo info_1 = {};
  info_1.base.name = "test1";
  info_1.base.return_type = "void";
  info_1.base.signature = "(int,uint32_t*)";
  info_1.base.parameters = {"int", "uint32_t*"};
  info_1.base.scope = {"TestNamespace"};
  info_1.base.is_constexpr = false;
  info_1.base.is_consteval = false;
  info_1.base.is_noexcept = false;
  info_1.base.is_deleted = false;
  info_1.is_extern = false;
  info_1.is_static = false;

  ScopeInfo info_2 = {};
  info_2.name = "TestNamespace";
  info_2.kind = ScopeInfo::Kind::kNamespace;
  info_2.full_scope = {"TestNamespace"};
  info_2.children = {};

  const std::string mock_label = "Test";
  const std::string mock_method_1 = "MOCK_METHOD2(test1, void(int,uint32_t*));";
  const std::string free_function_class = "class " + mock_label + "FreeFunction";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  std::shared_ptr<FunctionAttributeInterface> p_function_if(new NamespaceFunctionAttribute(info_1));
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes()).WillOnce(Return(std::vector<ScopeInfo>({info_2})));
  EXPECT_CALL(*p_analyzer, GetIncludes()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) == std::string::npos);
  EXPECT_TRUE(body.find(free_function_class) == std::string::npos);
}
