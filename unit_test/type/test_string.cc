#include <gtest/gtest.h>
#include <type/type.h>
#include <type/type_alias.h>
#include <iostream>

TEST(to_s, case1)
{
  Object* meta = MetaObject.New(10);
  Object* str = String.New(meta, "hoge");
  ASSERT_STREQ("hoge", String.to_s(str));
}
