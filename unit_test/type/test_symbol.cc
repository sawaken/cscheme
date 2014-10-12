#include <gtest/gtest.h>
#include <type/type.h>
#include <iostream>

TEST(to_i, case1)
{
  Object* meta = MetaObject.New(10);
  Object* symbol = Symbol.New(meta, "hoge");
  ASSERT_STREQ("hoge", Symbol.to_s(symbol));
}
