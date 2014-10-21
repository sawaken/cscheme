#include <gtest/gtest.h>
#include <type/type.h>
#include <cscheme.h>
#include <stdio.h>

TEST(Cscheme, case1)
{
  Interpreter* inter = Cscheme.New();

  Cscheme.eval(inter, "(+ 1 2 3)");

  Object* res = Cscheme.getEvaluated(inter);

  ASSERT_EQ(6, Integer.to_i(res));
}

TEST(Cscheme, case2)
{
  Interpreter* inter = Cscheme.New();

  Cscheme.eval(inter, "(if '#t' 2 3)");
  ASSERT_EQ(2, Integer.to_i(Cscheme.getEvaluated(inter)));

  Cscheme.eval(inter, "(if '#f' 2 3)");
  ASSERT_EQ(3, Integer.to_i(Cscheme.getEvaluated(inter)));

  Cscheme.eval(inter, "(if 1 2 3)");
  ASSERT_EQ(2, Integer.to_i(Cscheme.getEvaluated(inter)));
}

TEST(Cscheme, case3)
{
  Interpreter* inter = Cscheme.New();

  Cscheme.eval(inter, "((lambda (a b) (+ a b)) 1 2)");
  ASSERT_EQ(3, Integer.to_i(Cscheme.getEvaluated(inter)));
}

TEST(Cscheme, case4)
{
  Interpreter* inter = Cscheme.New();

  Cscheme.eval(inter, "(call/cc (lambda (c) (+ 1 (c 2) 3)))");
  ASSERT_EQ(2, Integer.to_i(Cscheme.getEvaluated(inter)));
}
