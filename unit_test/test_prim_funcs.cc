#include <gtest/gtest.h>
#include <type/type.h>
#include <type/type_alias.h>
#include <initial_bind.h>
#include <util.h>

#define C Continuation

class PFTest : public ::testing::Test {
protected:
  
  Object *meta, *env, *cont;
  Object *dummy1, *dummy2, *dummy3, *dummy4;

  virtual void SetUp() {
    meta = MetaObject.New(100);
    env = Env.New(meta, NULL);
    cont = C.New(meta, NULL);
    dummy1 = Dummy.New(meta, NULL);
    dummy2 = Dummy.New(meta, NULL);
    dummy3 = Dummy.New(meta, NULL);
    dummy4 = Dummy.New(meta, NULL);
    BindPF(meta, Util.singletonSymbol, env);
    MetaObject.referred(env);
  }
};

TEST_F(PFTest, sum_int)
{
  Object* sum_int = Env.find(env, Util.singletonSymbol(meta, "+"), Util.comp);
  Object* args[] = {Integer.New(meta, 1), Integer.New(meta, 2)};
  int argc = 2;

  Object* res = PrimFunc.apply(sum_int, meta, args, argc);

  ASSERT_EQ(3, Integer.to_i(res));
}
