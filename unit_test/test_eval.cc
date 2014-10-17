#include <gtest/gtest.h>
#include <type/type.h>
#include <util.h>
#include <eval.h>

#define C Continuation
#define D Dummy
#define F Form
typedef Object O;

class EvalTest : public ::testing::Test {
protected:

  O *meta, *cont;
  O *dummy1, *dummy2, *dummy3;
  O *env1, *env2;
  O *form1, *form2;
  virtual void SetUp() {
    meta = MetaObject.New(100);
    cont = C.New(meta, NULL);
    dummy1 = D.New(meta, NULL);
    dummy2 = D.New(meta, NULL);
    dummy3 = D.New(meta, NULL);
    env1 = Env.New(meta, NULL);
    env2 = Env.New(meta, env1);
    form1 = Util.form(meta, env1, false, 2, dummy1, dummy2);
    form2 = Util.form(meta, env2, true, 2, dummy1, dummy2);
  }
};


TEST_F(EvalTest, TailCallOptimize_cell_tail)
{
  O* tail = Util.list(meta, 2, dummy1, dummy2);

  C.push(cont, dummy3);
  TailCallOptimize(meta, cont, env1, tail);

  ASSERT_EQ(1, C.size(cont));
  ASSERT_EQ(2, F.restNum(C.top(cont)));
  ASSERT_EQ(dummy1, F.next(C.top(cont)));
}
