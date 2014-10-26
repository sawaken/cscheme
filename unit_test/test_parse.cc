#include <gtest/gtest.h>
#include <type/type.h>
#include <type/type_alias.h>
#include <parse.h>
#include <util.h>

#define car(cell) Cell.car(cell)
#define cdr(cell) Cell.cdr(cell)

class ParseTest : public ::testing::Test {
protected:

  Generator g;
  Object *meta;
  
  virtual void SetUp() {
    meta = MetaObject.New(100);
    g = (Generator){meta, Cell.New, Symbol.New, NULL};
  }
};


TEST_F(ParseTest, case1)
{
  Object* exp = ParseExp("(+ 1 2 3)", &g);
  ASSERT_EQ(1, Util.length(exp));
  ASSERT_EQ(4, Util.length(car(exp)));
}

TEST_F(ParseTest, case2)
{
  Object* exp = ParseExp("(+ (+ a b c) (+ a b c))", &g);
  ASSERT_EQ(1, Util.length(exp));
  ASSERT_EQ(3, Util.length(car(exp)));
  ASSERT_EQ(4, Util.length(car(cdr(car(exp)))));
  ASSERT_EQ(4, Util.length(car(cdr(cdr(car(exp))))));
}

TEST_F(ParseTest, case3)
{
  Object* exp = ParseExp("hoge", &g);
  ASSERT_EQ(1, Util.length(exp));
  ASSERT_TRUE(IsA(car(exp), &Symbol));
  ASSERT_STREQ("hoge", Symbol.to_s(car(exp)));
}
