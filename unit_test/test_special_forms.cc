#include <gtest/gtest.h>
#include <type/type.h>
#include <initial_bind.h>
#include <util.h>

class SFTest : public ::testing::Test {
protected:
  
  Generator g;
  Object *meta, *env;

  virtual void SetUp() {
    meta = MetaObject.New(100);
    env = Env.New(meta, NULL);
    g = (Generator){meta, Cell.New, Util.singletonSymbol, NULL};
    BindSF(&g, env);
  }
};

TEST_F(SFTest, _if)
{
  Object* sym1 = Util.singletonSymbol(meta, "hoge");
  Object* sym2 = Util.singletonSymbol(meta, "hoge");

  ASSERT_EQ(sym1, sym2);
}


