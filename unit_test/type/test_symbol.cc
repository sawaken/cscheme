#include <gtest/gtest.h>
#include <type/type.h>
#include <type/type_alias.h>
#include <iostream>

TEST(to_s, case1)
{
  Object* meta = MetaObject.New(10);
  Object* symbol = Symbol.New(meta, "hoge");
  ASSERT_STREQ("hoge", Symbol.to_s(symbol));
}
