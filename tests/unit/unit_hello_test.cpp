#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(UnitTestsHello, BasicAssertions_1)
{
   // Expect to br true
   EXPECT_TRUE(1);

   // Expect to br false
   EXPECT_FALSE(0);
}

TEST(UnitTestsHello, BasicAssertions_2)
{
   // Expect two strings not to be equal.
   EXPECT_STRNE("hello", "world");

   // Expect equality.
   EXPECT_EQ(7 * 6, 42);
}