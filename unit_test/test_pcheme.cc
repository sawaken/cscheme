#include <gtest/gtest.h>
#include <type/type.h>
#include <pcheme.h>
#include <stdio.h>

TEST(Pcheme, case1)
{
  Interpreter* inter = Pcheme.New();

  Pcheme.eval(inter, "(+ 1 2 3)");

  Object* res = Pcheme.getEvaluated(inter);

  ASSERT_EQ(6, Integer.to_i(res));
}

TEST(Pcheme, case2)
{
  Interpreter* inter = Pcheme.New();

  Pcheme.eval(inter, "(if '#t' 2 3)");
  ASSERT_EQ(2, Integer.to_i(Pcheme.getEvaluated(inter)));

  Pcheme.eval(inter, "(if '#f' 2 3)");
  ASSERT_EQ(3, Integer.to_i(Pcheme.getEvaluated(inter)));

  Pcheme.eval(inter, "(if 1 2 3)");
  ASSERT_EQ(2, Integer.to_i(Pcheme.getEvaluated(inter)));
}

TEST(Pcheme, case3)
{
  Interpreter* inter = Pcheme.New();

  Pcheme.eval(inter, "((lambda (a b) (+ a b)) 1 2)");
  ASSERT_EQ(3, Integer.to_i(Pcheme.getEvaluated(inter)));
}

TEST(Pcheme, case4)
{
  Interpreter* inter = Pcheme.New();

  Pcheme.eval(inter, "(call/cc (lambda (c) (+ 1 (c 2) 3)))");
  ASSERT_EQ(2, Integer.to_i(Pcheme.getEvaluated(inter)));
}
