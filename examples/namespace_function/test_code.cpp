#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>

#include "code1.h"
#include "mock_code2.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::Return;

TEST(namespace_function_test, foo) {
  EXPECT_CALL(mock_hoge::GetInstance(), namespace_function_bar(_, _)).WillOnce(Return(1));
  EXPECT_TRUE(namespace_function_foo(0));
}
