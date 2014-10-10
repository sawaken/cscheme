#include <gtest/gtest.h>
#include <type/type.h>
#include <iostream>

TEST(to_i, case1)
{
  Object* meta = MetaObject.New(10);
  Object* integer = Integer.New(meta, 1000);
  ASSERT_EQ(1000, Integer.to_i(integer));
}
