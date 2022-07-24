#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>

#include "code1.h"
#include "mock_code2_generated_by_gtgen.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::Return;

TEST(free_function_test, foo) {
  EXPECT_CALL(mock_FreeFunction::GetInstance(), free_function_bar(_, _)).WillOnce(Return(1));
  EXPECT_TRUE(free_function_foo(0));
}
