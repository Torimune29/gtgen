#include <gtest/gtest.h>
#include "AbstractCodeParser.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

TEST(AbstractCodeParserTest, GetPath) {
  AbstractCodeParser parser("test");
  EXPECT_STREQ(parser.GetPath().c_str(), "test");
}
