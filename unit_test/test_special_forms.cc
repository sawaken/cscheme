#include <gtest/gtest.h>
#include <type/type.h>
#include <initial_bind.h>
#include <util.h>

#define C Continuation

class SFTest : public ::testing::Test {
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
    BindSF(meta, Util.singletonSymbol, env);
    MetaObject.referred(env);
  }
};

TEST_F(SFTest, _if_with_3arg_true)
{
  Object* _if = Env.find(env, Util.singletonSymbol(meta, "if"), Util.comp);
  ASSERT_TRUE(_if != NULL);

  C.push(cont, Util.form(meta, env, false, 4, dummy1, dummy2, dummy3, dummy4));

  C.push(cont, _if);
  C.returnTopToForm(cont);

  C.push(cont, Bool.New(meta, true));
  C.returnTopToForm(cont);

  ASSERT_TRUE(SpecialForm.doAction(_if, meta, cont));

  ASSERT_EQ(1, C.size(cont));
  ASSERT_EQ(dummy3, C.top(cont));
}

TEST_F(SFTest, _if_with_3arg_false)
{
  Object* _if = Env.find(env, Util.singletonSymbol(meta, "if"), Util.comp);
  ASSERT_TRUE(_if != NULL);

  C.push(cont, Util.form(meta, env, false, 4, dummy1, dummy2, dummy3, dummy4));

  C.push(cont, _if);
  C.returnTopToForm(cont);

  C.push(cont, Bool.New(meta, false));
  C.returnTopToForm(cont);

  ASSERT_TRUE(SpecialForm.doAction(_if, meta, cont));

  ASSERT_EQ(1, C.size(cont));
  ASSERT_EQ(dummy4, C.top(cont));
}

TEST_F(SFTest, call_cc)
{
  Object* call_cc = Env.find(env, Util.singletonSymbol(meta, "call/cc"), Util.comp);
  ASSERT_TRUE(call_cc != NULL);

  C.push(cont, dummy1);
  C.push(cont, Util.form(meta, env, false, 2, dummy2, dummy3));

  C.push(cont, call_cc);
  C.returnTopToForm(cont);

  C.push(cont, dummy4);
  C.returnTopToForm(cont);

  ASSERT_TRUE(SpecialForm.doAction(call_cc, meta, cont));

  ASSERT_EQ(2, C.size(cont));
  ASSERT_EQ(dummy4, Form.rawElement(C.top(cont), 0));

  Object* cc = Form.rawElement(C.top(cont), 1);
  ASSERT_TRUE(IsA(cc, &Continuation));
  ASSERT_EQ(1, C.size(cc));
  ASSERT_EQ(dummy1, C.top(cc));
}

TEST_F(SFTest, lambda)
{
  Object* lambda = Env.find(env, Util.singletonSymbol(meta, "lambda"), Util.comp);
  ASSERT_TRUE(lambda != NULL);

  Object* param_list = Util.symList(meta, Util.singletonSymbol, 4, "a", "b", ".", "c");
  C.push(cont, Util.form(meta, env, false, 4, dummy1, param_list, dummy2, dummy3));

  C.push(cont, lambda);
  C.returnTopToForm(cont);

  ASSERT_TRUE(SpecialForm.doAction(lambda, meta, cont));

  ASSERT_EQ(1, C.size(cont));
  ASSERT_TRUE(IsA(C.top(cont), &Lambda));
  ASSERT_EQ(env, Lambda.env(C.top(cont)));
  ASSERT_EQ(2, Lambda.expc(C.top(cont)));
  ASSERT_EQ(dummy2, *(Lambda.exps(C.top(cont)) + 0));
  ASSERT_EQ(dummy3, *(Lambda.exps(C.top(cont)) + 1));
}
