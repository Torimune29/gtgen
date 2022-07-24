#include <gtest/gtest.h>

#include <memory>

#include "code1.h"
#include "mock_code2_generated_by_gtgen.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::Return;

TEST(class_inherit_function_test, foo) {
  mock_hogehoge hogee;
  EXPECT_CALL(hogee, class_inherit_function_bar(_, _)).Times(1).WillRepeatedly(Return(1));
  huga hugaa(&hogee);
  EXPECT_TRUE(hugaa.class_inherit_function_foo(0));
}
