#include <gtest/gtest.h>

/*
  Target Functions
*/

int add(int x, int y)
{
  return x + y;
}

char* strcopy(char* src, char* dest)
{
  for (int i = 0; ; i++) {
    dest[i] = src[i];
    if (src[i] == '\0') break;
  }
  return dest;
}

int XOR(int a, int b)
{
  return (a || b) && !(a && b);
}


/*
  Test Fixture
*/
class FooTest : public ::testing::Test {
protected:

  int a, b, c;

  virtual void SetUp() {
    a = 1, b = 2, c = 3;
  }
};


/*
  Assertions
*/

TEST(ASSERT_EXPECT, sample)
{
  EXPECT_EQ(1, 1); // continue test even if EXPECT fail.
  ASSERT_EQ(1, 1);
}

TEST(XORTest, sample)
{
  ASSERT_FALSE(XOR(0, 0));
  ASSERT_FALSE(XOR(1, 1));
  ASSERT_TRUE(XOR(0, 1));
  ASSERT_TRUE(XOR(1, 0));
}


TEST(AddTest, sample)
{
  ASSERT_EQ(2, add(1, 1));
  ASSERT_NE(3, add(1, 1));
  ASSERT_LT(1, add(1, 1));
  ASSERT_LE(2, add(1, 1));
}

TEST(StrTest, sample)
{
  char a[5] = "hoge", b[5] = "Hoge";

  ASSERT_STRNE(a, b);
  ASSERT_STRCASEEQ(a, b);
  ASSERT_STREQ(a, strcopy(a, b));
  ASSERT_STRCASENE("hoge", "fuga");
}

TEST_F(FooTest, sample)
{
  ASSERT_EQ(c, a + b);
}


