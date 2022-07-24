#include <gtest/gtest.h>

#include <memory>

#include "code1.h"
#include "mock_code2.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::Return;

TEST(class_inherit_function_test, foo) {
  std::shared_ptr<mock_hoge> p_hoge(new mock_hoge());
  EXPECT_CALL(*p_hoge, class_inherit_function_bar(_, _)).WillOnce(Return(1));
  huga hugaa(p_hoge.get());
  EXPECT_TRUE(hugaa.class_inherit_function_foo(0));
}
