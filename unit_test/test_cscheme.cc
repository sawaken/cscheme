#include <gtest/gtest.h>
#include <type/type.h>
#include <type/type_alias.h>
#include <cscheme.h>
#include <stdio.h>

TEST(CSCM, case1)
{
  CSCM_Interpreter* inter = CSCM.New();

  CSCM.eval(inter, "(+ 1 2 3)");

  Object* res = CSCM.getEvaluated(inter);

  ASSERT_EQ(6, Integer.to_i(res));
}

TEST(CSCM, case2)
{
  CSCM_Interpreter* inter = CSCM.New();

  CSCM.eval(inter, "(if '#t' 2 3)");
  ASSERT_EQ(2, Integer.to_i(CSCM.getEvaluated(inter)));

  CSCM.eval(inter, "(if '#f' 2 3)");
  ASSERT_EQ(3, Integer.to_i(CSCM.getEvaluated(inter)));

  CSCM.eval(inter, "(if 1 2 3)");
  ASSERT_EQ(2, Integer.to_i(CSCM.getEvaluated(inter)));
}

TEST(CSCM, case3)
{
  CSCM_Interpreter* inter = CSCM.New();

  CSCM.eval(inter, "((lambda (a b) (+ a b)) 1 2)");
  ASSERT_EQ(3, Integer.to_i(CSCM.getEvaluated(inter)));
}

TEST(CSCM, case4)
{
  CSCM_Interpreter* inter = CSCM.New();

  CSCM.eval(inter, "(call/cc (lambda (c) (+ 1 (c 2) 3)))");
  ASSERT_EQ(2, Integer.to_i(CSCM.getEvaluated(inter)));
}
