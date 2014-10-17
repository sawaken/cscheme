#include <gtest/gtest.h>
#include <type/type.h>

TEST(test, case1)
{
  Object* meta = MetaObject.New(100);
  Object* dummy_param = Dummy.New(meta, NULL);

  Object* exps[3];
  exps[0] = Dummy.New(meta, NULL);
  exps[1] = Dummy.New(meta, NULL);
  exps[2] = Dummy.New(meta, NULL);

  Object* lambda = Lambda.New(meta, dummy_param, exps, 3);

  ASSERT_EQ(dummy_param, Lambda.param(lambda));
  ASSERT_EQ(3, Lambda.expc(lambda));
  ASSERT_EQ(exps[0], *(Lambda.exps(lambda) + 0));
  ASSERT_EQ(exps[1], *(Lambda.exps(lambda) + 1));
  ASSERT_EQ(exps[2], *(Lambda.exps(lambda) + 2));

  MetaObject.release(lambda);
  
  ASSERT_TRUE(Dummy.isReleased(dummy_param));
  ASSERT_TRUE(Dummy.isReleased(exps[0]));
  ASSERT_TRUE(Dummy.isReleased(exps[1]));
  ASSERT_TRUE(Dummy.isReleased(exps[2]));
}
