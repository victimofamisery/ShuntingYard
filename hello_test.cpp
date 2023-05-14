#include "shuntingyard.h"
#include <gtest/gtest.h>
// Demonstrate some basic assertions.
TEST(HelloTest, addition) {
  EXPECT_EQ(ShuntingYard::calc("9+3"), 12);
}

TEST(HelloTest, asin) {
  EXPECT_EQ(ShuntingYard::calc("asin(0)"), asin(0));
}

TEST(HelloTest, complex) {
  EXPECT_EQ(ShuntingYard::calc("3+5 - 10   *  15/ 3"), -42);
}