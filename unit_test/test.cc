#include <gtest/gtest.h>

int add(int x, int y)
{
  return x + y;
}

TEST(AddTest, t1)
{
  ASSERT_EQ(2, add(1, 1));
  ASSERT_EQ(3, add(1, 1));
}
