#include <gtest/gtest.h>
#include <type/type.h>

bool eq(Object* a, Object* b)
{
  return a == b;
}

TEST(test, bind_and_find)
{
  Object* meta = MetaObject.New(100);
  Object* env1 = Env.New(meta, NULL);
  Object* env2 = Env.New(meta, env1);

  Object* key1 = Dummy.New(meta, NULL);
  Object* key2 = Dummy.New(meta, NULL);
  Object* key3 = Dummy.New(meta, NULL);

  Object* val1 = Dummy.New(meta, NULL);
  Object* val2 = Dummy.New(meta, NULL);

  Env.bind(env1, key1, val1);
  Env.bind(env2, key2, val2);

  ASSERT_EQ(1, Env.size(env1));
  ASSERT_EQ(1, Env.size(env2));
  
  ASSERT_EQ(val1, Env.find(env1, key1, eq));
  ASSERT_EQ(NULL, Env.find(env1, key2, eq));
  ASSERT_EQ(NULL, Env.find(env1, key3, eq));

  ASSERT_EQ(val1, Env.find(env2, key1, eq));
  ASSERT_EQ(val2, Env.find(env2, key2, eq));
  ASSERT_EQ(NULL, Env.find(env2, key3, eq));
}
