#include <gtest/gtest.h>
#include <type/type.h>

TEST(test, case1)
{
  Object* meta = MetaObject.New(100);
  Object* env1 = Env.New(meta, NULL);
  Object* env2 = Env.New(meta, env1);

  
}
