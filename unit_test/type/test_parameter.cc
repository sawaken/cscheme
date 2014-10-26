#include <gtest/gtest.h>
#include <type/type.h>
#include <type/type_alias.h>

TEST(test, without_rest)
{
  Object* meta = MetaObject.New(100);
  Object* p[2];

  p[0] = Dummy.New(meta, NULL);
  p[1] = Dummy.New(meta, NULL);

  Object* param = Parameter.New(meta, p, 2, NULL);

  ASSERT_EQ(2, Parameter.paramc(param));
  ASSERT_EQ(p[0], *(Parameter.params(param) + 0));
  ASSERT_EQ(p[1], *(Parameter.params(param) + 1));
  ASSERT_EQ(NULL, Parameter.rest(param));
  

  ASSERT_FALSE(Parameter.validArgLength(param, 1));
  ASSERT_TRUE(Parameter.validArgLength(param, 2));
  ASSERT_FALSE(Parameter.validArgLength(param, 3));
}

TEST(test, with_rest)
{
  Object* meta = MetaObject.New(100);
  Object* p[3];

  p[0] = Dummy.New(meta, NULL);
  p[1] = Dummy.New(meta, NULL);
  p[2] = Dummy.New(meta, NULL);

  Object* param = Parameter.New(meta, p, 2, p[2]);

  ASSERT_EQ(p[2], Parameter.rest(param));
  
  ASSERT_FALSE(Parameter.validArgLength(param, 1));
  ASSERT_TRUE(Parameter.validArgLength(param, 2));
  ASSERT_TRUE(Parameter.validArgLength(param, 3));
}
