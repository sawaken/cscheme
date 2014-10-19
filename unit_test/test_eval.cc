#include <gtest/gtest.h>
#include <type/type.h>
#include <util.h>
#include <eval.h>

#define C Continuation
#define D Dummy
#define F Form
typedef Object O;

class EvalSubTest : public ::testing::Test {
protected:

  O *meta, *cont;
  O *dummy1, *dummy2, *dummy3;
  O *env1, *env2;
  O *form1, *form2;
  O *sym1, *sym2, *sym3;
  virtual void SetUp() {
    meta = MetaObject.New(100);
    cont = C.New(meta, NULL);
    dummy1 = D.New(meta, NULL);
    dummy2 = D.New(meta, NULL);
    dummy3 = D.New(meta, NULL);
    env1 = Env.New(meta, NULL);
    env2 = Env.New(meta, env1);
    form1 = Util.form(meta, env1, false, 2, dummy1, dummy2);
    form2 = Util.form(meta, env2, true, 3, dummy1, dummy2, dummy3);
    sym1 = Symbol.New(meta, "a");
    sym2 = Symbol.New(meta, "b");
    sym3 = Symbol.New(meta, "c");
  }
};


TEST_F(EvalSubTest, TailCallOptimize_cell_tail)
{
  O* tail = Util.list(meta, 2, dummy1, dummy2);

  C.push(cont, dummy3);
  TailCallOptimize(meta, cont, env1, tail);

  ASSERT_EQ(1, C.size(cont));
  ASSERT_EQ(2, F.restNum(C.top(cont)));
  ASSERT_EQ(dummy1, F.next(C.top(cont)));
}

TEST_F(EvalSubTest, TailCallOptimize_noncell_tail)
{
  O* tail = D.New(meta, NULL);

  C.push(cont, dummy3);
  TailCallOptimize(meta, cont, env1, tail);

  ASSERT_EQ(1, C.size(cont));
  ASSERT_EQ(tail, C.top(cont));
}

TEST_F(EvalSubTest, StackNextFrame_with_cell)
{
  O* next = Util.list(meta, 2, dummy1, dummy2);

  StackNextFrame(meta, cont, env1, next);
  
  ASSERT_EQ(1, C.size(cont));
  ASSERT_EQ(2, F.restNum(C.top(cont)));
  ASSERT_EQ(dummy1, F.next(C.top(cont)));
}

TEST_F(EvalSubTest, StackNextFrame_with_bound_symbol)
{
  O* next = Symbol.New(meta, "hoge");

  Env.bind(env1, next, dummy1);
  StackNextFrame(meta, cont, env1, next);
  
  ASSERT_EQ(1, C.size(cont));
  ASSERT_EQ(dummy1, C.top(cont));
}

TEST_F(EvalSubTest, StackNextFrame_with_unbound_symbol)
{
  O* next = Symbol.New(meta, "hoge");

  StackNextFrame(meta, cont, env1, next);
  
  ASSERT_EQ(1, C.size(cont));
  ASSERT_TRUE(IsA(C.top(cont), &Exception));
}

TEST_F(EvalSubTest, StackNextFrame_with_object)
{
  StackNextFrame(meta, cont, env1, dummy1);
  
  ASSERT_EQ(1, C.size(cont));
  ASSERT_EQ(dummy1, C.top(cont));
}

TEST_F(EvalSubTest, ApplyContinuation_with_valid_arg)
{
  O* alt_cont = C.New(meta, NULL);
  C.push(alt_cont, form2);
  F.back(form1, dummy1);
  F.back(form1, dummy2);

  ApplyContinuation(meta, cont, alt_cont, form1);
  
  ASSERT_EQ(2, C.size(cont));
  ASSERT_EQ(dummy2, C.top(cont));
  ASSERT_EQ(form2, C.at(cont, 0));
}

TEST_F(EvalSubTest, ApplyContinuation_with_invalid_arg)
{
  O* alt_cont = C.New(meta, NULL);
  F.back(form2, dummy1);
  F.back(form2, dummy2);
  F.back(form2, dummy3);

  ApplyContinuation(meta, cont, alt_cont, form2);
  
  ASSERT_EQ(1, C.size(cont));
  ASSERT_TRUE(IsA(C.top(cont), &Exception));
}

TEST_F(EvalSubTest, MakeForm)
{
  O* p[3] = {sym1, sym2, sym3};
  O* e[2] = {form1, form2};
  O* a[3] = {dummy1, dummy2, dummy3};

  O* param  = Parameter.New(meta, p, 2, p[2]);
  O* lambda = Lambda.New(meta, env1, param, e, 2);
  O* form = MakeForm(meta, lambda, a, 3);

  ASSERT_EQ(2, F.restNum(form));
  ASSERT_EQ(form1, F.rawElement(form, 0));
  ASSERT_EQ(form2, F.rawElement(form, 1));

  ASSERT_EQ(dummy1, Env.find(F.env(form), sym1, Util.comp));
  ASSERT_EQ(dummy2, Env.find(F.env(form), sym2, Util.comp));
  ASSERT_EQ(1, Util.length(Env.find(F.env(form), sym3, Util.comp)));
  ASSERT_EQ(dummy3, Cell.car(Env.find(F.env(form), sym3, Util.comp)));
}

TEST_F(EvalSubTest, Apply_fail)
{
  F.back(form1, dummy1);
  F.back(form1, dummy2);

  Apply(meta, cont, form1);

  ASSERT_EQ(1, C.size(cont));
  ASSERT_TRUE(IsA(C.top(cont), &Exception));
}

TEST_F(EvalSubTest, Raise)
{
  O* exception = Exception.New(meta, dummy3);
  C.push(cont, dummy1);
  C.push(cont, dummy2);
  C.push(cont, exception); 

  Raise(meta, cont);

  ASSERT_EQ(1, C.size(cont));
  ASSERT_EQ(exception, C.top(cont));
}

O* add(Object* meta, Object** args, int argc) {
  return Integer.New(meta, Integer.to_i(args[0]) + Integer.to_i(args[1]));
}

TEST_F(EvalSubTest, simple_case)
{
  // let a := +, b := 1, c := 2
  // calc (a (a b b) (a c c))
  O* exp1 = Util.list(meta, 3, sym1, sym2, sym2);
  O* exp2 = Util.list(meta, 3, sym1, sym3, sym3);

  O* form = Util.form(meta, env1, false, 3, sym1, exp1, exp2);
  Env.bind(env1, sym1, PrimFunc.New(meta, "+", add));
  Env.bind(env1, sym2, Integer.New(meta, 1));
  Env.bind(env1, sym3, Integer.New(meta, 2));
  C.push(cont, form);

  Eval(meta, cont);

  ASSERT_EQ(1, C.size(cont));
  ASSERT_TRUE(IsA(C.top(cont), &Integer));
  ASSERT_EQ(6, Integer.to_i(C.top(cont)));
}
