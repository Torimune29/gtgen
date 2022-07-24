#include <gtest/gtest.h>

#include "GoogleMockHarness.h"
#include "gtgen.h"
#include "mocks/mock_CodeAnalyzerInterface.h"
#include "mocks/mock_FunctionAttribute.h"

using ::testing::Return;

const std::string kSourceTreePath = SOURCE_DIR;

TEST(GoogleMockHarness, CreateWithMock) {
  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions_noexcept())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>()));
  EXPECT_CALL(*p_analyzer, GetScopes_noexcept()).WillOnce(Return(std::vector<ScopeInfo>()));
  EXPECT_CALL(*p_analyzer, GetIncludes_noexcept()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  std::cout << harness.Generate() << std::endl;
}

TEST(GoogleMockHarness, FreeFunctions) {
  std::shared_ptr<mock_FunctionAttributeInterface> p_mock_function_if(new mock_FunctionAttributeInterface());
  EXPECT_CALL(*p_mock_function_if, Name_noexcept()).WillRepeatedly(Return("test1"));
  EXPECT_CALL(*p_mock_function_if, ReturnType_noexcept()).WillOnce(Return("void"));
  EXPECT_CALL(*p_mock_function_if, Signature_noexcept()).WillOnce(Return("(int,uint32_t*)"));
  EXPECT_CALL(*p_mock_function_if, ParameterTypes_noexcept())
      .WillOnce(Return(std::vector<std::string>({"int", "uint32_t*"})));
  EXPECT_CALL(*p_mock_function_if, Parameters_noexcept())
      .WillRepeatedly(Return(std::vector<std::pair<std::string, std::string>>({
          std::make_pair("int", "a"),
          std::make_pair("uint32_t *", "b"),
      })));
  EXPECT_CALL(*p_mock_function_if, Scope_noexcept())
      .WillRepeatedly(Return(FunctionScope(std::vector<std::string>({""}))));
  EXPECT_CALL(*p_mock_function_if, Declaration_noexcept()).WillRepeatedly(Return("void test1 (int a, uint32_t *b)"));
  EXPECT_CALL(*p_mock_function_if, ExceptionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, IsOverloadedOperator_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, DefinitionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, IsClassMember_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, IsAbleToPolymorphic_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, CvQualifier_noexcept())
      .Times(2)
      .WillRepeatedly(Return(""));  // volatile and const check

  const std::string mock_label = "Test";
  const std::string mock_method_1 = "MOCK_METHOD2(test1, void(int,uint32_t*));";
  const std::string getinstance_method_declaration = mock_label + "FreeFunction& GetInstance()";
  const std::string getinstance_method_definition_1 = "static " + mock_label + "FreeFunction instance;";
  const std::string getinstance_method_definition_2 = "return instance;";
  const std::string free_function_class = "class " + mock_label + "FreeFunction";
  // const std::string free_function_macro = "#undef test1";
  const std::string free_function_stub =
      "void test1 (int a, uint32_t *b) {\n  return " + mock_label + "FreeFunction::GetInstance().test1(a, b);\n}";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions_noexcept())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_mock_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes_noexcept()).WillOnce(Return(std::vector<ScopeInfo>()));
  EXPECT_CALL(*p_analyzer, GetIncludes_noexcept()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) != std::string::npos);
  EXPECT_TRUE(body.find(free_function_class) != std::string::npos);
  EXPECT_TRUE(body.find(getinstance_method_declaration) != std::string::npos);
  EXPECT_TRUE(body.find(getinstance_method_definition_1) != std::string::npos);
  EXPECT_TRUE(body.find(getinstance_method_definition_2) != std::string::npos);
  // EXPECT_TRUE(body.find(free_function_macro) != std::string::npos);
  EXPECT_TRUE(body.find(free_function_stub) != std::string::npos);
}

TEST(GoogleMockHarness, ClassMemberFunctionsConst) {
  std::shared_ptr<mock_FunctionAttributeInterface> p_mock_function_if(new mock_FunctionAttributeInterface());
  EXPECT_CALL(*p_mock_function_if, Name_noexcept()).WillRepeatedly(Return("test1"));
  EXPECT_CALL(*p_mock_function_if, ReturnType_noexcept()).WillOnce(Return("void"));
  EXPECT_CALL(*p_mock_function_if, Signature_noexcept()).WillOnce(Return("(int,uint32_t*)"));
  EXPECT_CALL(*p_mock_function_if, Parameters_noexcept())
      .Times(2)
      .WillRepeatedly(Return(std::vector<std::pair<std::string, std::string>>({
          std::make_pair("int", "a"),
          std::make_pair("uint32_t *", "b"),
      })));
  EXPECT_CALL(*p_mock_function_if, ParameterTypes_noexcept())
      .WillOnce(Return(std::vector<std::string>({"int", "uint32_t*"})));
  EXPECT_CALL(*p_mock_function_if, Scope_noexcept())
      .WillRepeatedly(Return(FunctionScope(std::vector<std::string>({"TestClass1"}))));
  EXPECT_CALL(*p_mock_function_if, Declaration_noexcept())
      .WillRepeatedly(Return("void TestClass1::test1 (int a, uint32_t * b) const"));
  EXPECT_CALL(*p_mock_function_if, ExceptionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, IsOverloadedOperator_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, DefinitionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, IsClassMember_noexcept()).WillOnce(Return(true));
  EXPECT_CALL(*p_mock_function_if, AccessSpecifier_noexcept()).WillOnce(Return("public"));
  EXPECT_CALL(*p_mock_function_if, IsAbleToPolymorphic_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, CvQualifier_noexcept())
      .Times(2)
      .WillRepeatedly(Return("const"));  // volatile and const check

  ScopeInfo info_2 = {};
  info_2.name = "TestClass1";
  info_2.kind = ScopeInfo::Kind::kClass;
  info_2.full_scope = {"TestClass1"};
  info_2.children = {};

  const std::string mock_label = "Test";
  const std::string mock_method_1 = "MOCK_CONST_METHOD2(test1, void(int,uint32_t*));";
  const std::string function_class = "class " + mock_label + "TestClass1";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions_noexcept())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_mock_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes_noexcept()).WillOnce(Return(std::vector<ScopeInfo>({info_2})));
  EXPECT_CALL(*p_analyzer, GetIncludes_noexcept()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) != std::string::npos);
  EXPECT_TRUE(body.find(function_class) != std::string::npos);
}

TEST(GoogleMockHarness, ClassMemberFunctionsPrivate) {
  std::shared_ptr<mock_FunctionAttributeInterface> p_mock_function_if(new mock_FunctionAttributeInterface());
  EXPECT_CALL(*p_mock_function_if, DefinitionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, IsClassMember_noexcept()).WillOnce(Return(true));
  EXPECT_CALL(*p_mock_function_if, AccessSpecifier_noexcept()).WillOnce(Return("private"));
  EXPECT_CALL(*p_mock_function_if, Scope_noexcept())
      .WillRepeatedly(Return(FunctionScope(std::vector<std::string>({"TestClass1"}))));

  ScopeInfo info_2 = {};
  info_2.name = "TestClass1";
  info_2.kind = ScopeInfo::Kind::kClass;
  info_2.full_scope = {"TestClass1"};
  info_2.children = {};

  const std::string mock_label = "Test";
  const std::string mock_method_1 = "MOCK_CONST_METHOD2(test1, void(int,uint32_t*));";
  const std::string function_class = "class " + mock_label + "TestClass1";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions_noexcept())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_mock_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes_noexcept()).WillOnce(Return(std::vector<ScopeInfo>({info_2})));
  EXPECT_CALL(*p_analyzer, GetIncludes_noexcept()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) == std::string::npos);
  EXPECT_TRUE(body.find(function_class) == std::string::npos);
}

TEST(GoogleMockHarness, ClassMemberFunctionsDeleted) {
  std::shared_ptr<mock_FunctionAttributeInterface> p_mock_function_if(new mock_FunctionAttributeInterface());
  EXPECT_CALL(*p_mock_function_if, DefinitionSuffix_noexcept()).WillOnce(Return("delete"));
  EXPECT_CALL(*p_mock_function_if, Scope_noexcept())
      .WillRepeatedly(Return(FunctionScope(std::vector<std::string>({"TestClass1"}))));

  ScopeInfo info_2 = {};
  info_2.name = "TestClass1";
  info_2.kind = ScopeInfo::Kind::kClass;
  info_2.full_scope = {"TestClass1"};
  info_2.children = {};

  const std::string mock_label = "Test";
  const std::string mock_method_1 = "MOCK_CONST_METHOD2(test1, void(int,uint32_t*));";
  const std::string function_class = "class " + mock_label + "TestClass1";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions_noexcept())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_mock_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes_noexcept()).WillOnce(Return(std::vector<ScopeInfo>({info_2})));
  EXPECT_CALL(*p_analyzer, GetIncludes_noexcept()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) == std::string::npos);
  EXPECT_TRUE(body.find(function_class) == std::string::npos);
}

/**
 * @brief Construct a new TEST object
 * @warning function in namespace scoped is unsupported
 *
 */
TEST(GoogleMockHarness, NamespaceFunctions) {
  std::shared_ptr<mock_FunctionAttributeInterface> p_mock_function_if(new mock_FunctionAttributeInterface());
  EXPECT_CALL(*p_mock_function_if, Name_noexcept()).WillRepeatedly(Return("test1"));
  EXPECT_CALL(*p_mock_function_if, ReturnType_noexcept()).WillOnce(Return("void"));
  EXPECT_CALL(*p_mock_function_if, Signature_noexcept()).WillOnce(Return("(int,uint32_t*)"));
  EXPECT_CALL(*p_mock_function_if, ParameterTypes_noexcept())
      .WillOnce(Return(std::vector<std::string>({"int", "uint32_t*"})));
  EXPECT_CALL(*p_mock_function_if, Scope_noexcept())
      .WillRepeatedly(Return(FunctionScope(std::vector<std::string>({"TestNamespace"}))));
  EXPECT_CALL(*p_mock_function_if, ExceptionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, Parameters_noexcept())
      .Times(2)
      .WillRepeatedly(Return(std::vector<std::pair<std::string, std::string>>({
          std::make_pair("int", "a"),
          std::make_pair("uint32_t *", "b"),
      })));
  EXPECT_CALL(*p_mock_function_if, Declaration_noexcept())
      .WillRepeatedly(Return("void TestNamespace::test1 (int a, uint32_t * b)"));

  EXPECT_CALL(*p_mock_function_if, IsOverloadedOperator_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, DefinitionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, IsClassMember_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, IsAbleToPolymorphic_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, CvQualifier_noexcept())
      .Times(2)
      .WillRepeatedly(Return(""));  // volatile and const check

  ScopeInfo info_2 = {};
  info_2.name = "TestNamespace";
  info_2.kind = ScopeInfo::Kind::kNamespace;
  info_2.full_scope = {"TestNamespace"};
  info_2.children = {};

  const std::string mock_label = "Test";
  const std::string mock_method_1 = "MOCK_METHOD2(test1, void(int,uint32_t*));";
  const std::string free_function_class = "class " + mock_label + "FreeFunction";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions_noexcept())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_mock_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes_noexcept()).WillOnce(Return(std::vector<ScopeInfo>({info_2})));
  EXPECT_CALL(*p_analyzer, GetIncludes_noexcept()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) != std::string::npos);
  EXPECT_TRUE(body.find(free_function_class) == std::string::npos);
}

TEST(GoogleMockHarness, ClassMemberFunctionsInNamespace) {
  std::shared_ptr<mock_FunctionAttributeInterface> p_mock_function_if(new mock_FunctionAttributeInterface());
  EXPECT_CALL(*p_mock_function_if, Name_noexcept()).WillRepeatedly(Return("test1"));
  EXPECT_CALL(*p_mock_function_if, ReturnType_noexcept()).WillOnce(Return("void"));
  EXPECT_CALL(*p_mock_function_if, Signature_noexcept()).WillOnce(Return("(int,uint32_t*)"));
  EXPECT_CALL(*p_mock_function_if, ParameterTypes_noexcept())
      .WillOnce(Return(std::vector<std::string>({"int", "uint32_t*"})));
  EXPECT_CALL(*p_mock_function_if, Scope_noexcept())
      .WillRepeatedly(Return(FunctionScope(std::vector<std::string>({"TestNamespace", "TestClass1"}))));
  EXPECT_CALL(*p_mock_function_if, ExceptionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, Parameters_noexcept())
      .Times(2)
      .WillRepeatedly(Return(std::vector<std::pair<std::string, std::string>>({
          std::make_pair("int", "a"),
          std::make_pair("uint32_t *", "b"),
      })));
  EXPECT_CALL(*p_mock_function_if, Declaration_noexcept())
      .WillRepeatedly(Return("void TestNamespace::TestClass1::test1 (int a, uint32_t * b)"));
  EXPECT_CALL(*p_mock_function_if, IsOverloadedOperator_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, DefinitionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, IsClassMember_noexcept()).WillOnce(Return(true));
  EXPECT_CALL(*p_mock_function_if, AccessSpecifier_noexcept()).WillOnce(Return("public"));
  EXPECT_CALL(*p_mock_function_if, IsAbleToPolymorphic_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, CvQualifier_noexcept())
      .Times(2)
      .WillRepeatedly(Return(""));  // volatile and const check

  ScopeInfo info_2 = {}, info_2_1 = {};
  info_2.name = "TestNamespace";
  info_2.kind = ScopeInfo::Kind::kNamespace;
  info_2.full_scope = {"TestNamespace"};
  info_2_1.name = "TestClass1";
  info_2_1.kind = ScopeInfo::Kind::kClass;
  info_2_1.full_scope = {info_2.name, "TestClass1"};
  info_2_1.name = "TestClass1";
  info_2.children = {info_2_1};

  const std::string mock_label = "Test";
  const std::string mock_method_1 = "MOCK_METHOD2(test1, void(int,uint32_t*));";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions_noexcept())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_mock_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes_noexcept()).WillOnce(Return(std::vector<ScopeInfo>({info_2})));
  EXPECT_CALL(*p_analyzer, GetIncludes_noexcept()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) != std::string::npos);
}

TEST(GoogleMockHarness, NoexceptFunctionsWorkaround) {
  std::shared_ptr<mock_FunctionAttributeInterface> p_mock_function_if(new mock_FunctionAttributeInterface());
  EXPECT_CALL(*p_mock_function_if, Name_noexcept()).WillRepeatedly(Return("test1"));
  EXPECT_CALL(*p_mock_function_if, ReturnType_noexcept()).WillOnce(Return("void"));
  EXPECT_CALL(*p_mock_function_if, Signature_noexcept()).WillOnce(Return("(int,uint32_t*)"));
  EXPECT_CALL(*p_mock_function_if, ParameterTypes_noexcept())
      .WillOnce(Return(std::vector<std::string>({"int", "uint32_t*"})));
  EXPECT_CALL(*p_mock_function_if, Parameters_noexcept())
      .Times(4)
      .WillRepeatedly(Return(std::vector<std::pair<std::string, std::string>>({
          std::make_pair("int", "a"),
          std::make_pair("uint32_t *", "b"),
      })));
  EXPECT_CALL(*p_mock_function_if, Declaration_noexcept())
      .WillRepeatedly(Return("void test1 (int a, uint32_t * b) const noexcept"));
  EXPECT_CALL(*p_mock_function_if, Scope_noexcept())
      .WillRepeatedly(Return(FunctionScope(std::vector<std::string>({"TestClass1"}))));
  EXPECT_CALL(*p_mock_function_if, ExceptionSuffix_noexcept()).WillOnce(Return("noexcept"));
  EXPECT_CALL(*p_mock_function_if, IsOverloadedOperator_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, DefinitionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, IsClassMember_noexcept()).WillOnce(Return(true));
  EXPECT_CALL(*p_mock_function_if, AccessSpecifier_noexcept()).WillOnce(Return("public"));
  EXPECT_CALL(*p_mock_function_if, IsAbleToPolymorphic_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, CvQualifier_noexcept())
      .Times(2)
      .WillRepeatedly(Return("const"));  // volatile and const check

  ScopeInfo info_2 = {};
  info_2.name = "TestClass1";
  info_2.kind = ScopeInfo::Kind::kClass;
  info_2.full_scope = {"TestClass1"};
  info_2.children = {};

  const std::string mock_label = "Test";
  const std::string mock_method_1 =
      R"xxx(MOCK_CONST_METHOD2(test1_noexcept, void(int,uint32_t*));
  void test1 (int a, uint32_t * b) const noexcept {
    return this->test1_noexcept(a, b);
  })xxx";
  const std::string function_class = "class " + mock_label + "TestClass1";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions_noexcept())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_mock_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes_noexcept()).WillOnce(Return(std::vector<ScopeInfo>({info_2})));
  EXPECT_CALL(*p_analyzer, GetIncludes_noexcept()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) != std::string::npos);
  EXPECT_TRUE(body.find(function_class) != std::string::npos);
}

TEST(GoogleMockHarness, OverloadedOperatorFunctionsWorkaround) {
  std::shared_ptr<mock_FunctionAttributeInterface> p_mock_function_if(new mock_FunctionAttributeInterface());
  EXPECT_CALL(*p_mock_function_if, Name_noexcept())
      .WillRepeatedly(Return("operator<<"));  // creatng method and generating hash
  EXPECT_CALL(*p_mock_function_if, ReturnType_noexcept()).WillOnce(Return("void"));
  EXPECT_CALL(*p_mock_function_if, Signature_noexcept()).WillOnce(Return("(int,uint32_t*)"));
  EXPECT_CALL(*p_mock_function_if, ParameterTypes_noexcept())
      .WillOnce(Return(std::vector<std::string>({"int", "uint32_t*"})));
  EXPECT_CALL(*p_mock_function_if, Parameters_noexcept())
      .WillRepeatedly(Return(std::vector<std::pair<std::string, std::string>>({
          std::make_pair("int", "a"),
          std::make_pair("uint32_t *", "b"),
      })));
  EXPECT_CALL(*p_mock_function_if, Declaration_noexcept())
      .WillRepeatedly(Return("void operator<< (int a, uint32_t * b) const noexcept"));
  EXPECT_CALL(*p_mock_function_if, Scope_noexcept())
      .WillRepeatedly(Return(FunctionScope(std::vector<std::string>({"TestClass1"}))));
  EXPECT_CALL(*p_mock_function_if, IsOverloadedOperator_noexcept()).WillOnce(Return(true));
  EXPECT_CALL(*p_mock_function_if, DefinitionSuffix_noexcept()).WillOnce(Return(""));
  EXPECT_CALL(*p_mock_function_if, IsClassMember_noexcept()).WillOnce(Return(true));
  EXPECT_CALL(*p_mock_function_if, AccessSpecifier_noexcept()).WillOnce(Return("public"));
  EXPECT_CALL(*p_mock_function_if, IsAbleToPolymorphic_noexcept()).WillOnce(Return(false));
  EXPECT_CALL(*p_mock_function_if, CvQualifier_noexcept())
      .Times(2)
      .WillRepeatedly(Return("const"));  // volatile and const check

  ScopeInfo info_2 = {};
  info_2.name = "TestClass1";
  info_2.kind = ScopeInfo::Kind::kClass;
  info_2.full_scope = {"TestClass1"};
  info_2.children = {};

  const std::string mock_label = "Test";
  const std::string mock_method_1 =
      R"xxx(MOCK_CONST_METHOD2(operator_2395791331043117477, void(int,uint32_t*));
  void operator<< (int a, uint32_t * b) const noexcept {
    return this->operator_2395791331043117477(a, b);
  })xxx";
  const std::string function_class = "class " + mock_label + "TestClass1";

  std::shared_ptr<mock_CodeAnalyzerInterface> p_analyzer(new mock_CodeAnalyzerInterface());
  GoogleMockHarness harness("Test", p_analyzer);
  EXPECT_CALL(*p_analyzer, GetFunctions_noexcept())
      .WillOnce(Return(std::vector<std::shared_ptr<FunctionAttributeInterface>>({p_mock_function_if})));
  EXPECT_CALL(*p_analyzer, GetScopes_noexcept()).WillOnce(Return(std::vector<ScopeInfo>({info_2})));
  EXPECT_CALL(*p_analyzer, GetIncludes_noexcept()).WillOnce(Return(std::vector<IncludeInfo>()));
  EXPECT_TRUE(harness.Ready());
  auto body = harness.Generate();
  std::cout << body << std::endl;
  EXPECT_TRUE(body.find(mock_method_1) != std::string::npos);
  EXPECT_TRUE(body.find(function_class) != std::string::npos);
}
