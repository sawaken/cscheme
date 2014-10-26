#include <gtest/gtest.h>
#include <util.h>
#include <type/type_alias.h>

#define I(n) Integer.New(meta, (n))
#define GI(e) Integer.to_i(e)
#define car(e) Cell.car(e)
#define cdr(e) Cell.cdr(e)

#define GS(e) Symbol.to_s(e)


TEST(New, case1)
{
  Object* meta = MetaObject.New(10);
  Object* ls = Util.list(meta, 2, I(0), I(1));

  ASSERT_FALSE(Cell.empty(ls));
  ASSERT_EQ(0, GI(car(ls)));

  Object* c = cdr(ls);
  ASSERT_FALSE(Cell.empty(c));
  ASSERT_EQ(1, GI(car(c)));

  Object* cc = cdr(c);
  ASSERT_TRUE(Cell.empty(cc));
}

TEST(symList, case1)
{
  Object* meta = MetaObject.New(10);
  Object* ls = Util.symList(meta, Util.singletonSymbol, 2, "hoge", "fuga");

  ASSERT_FALSE(Cell.empty(ls));
  ASSERT_STREQ("hoge", GS(car(ls)));

  Object* c = cdr(ls);
  ASSERT_FALSE(Cell.empty(c));
  ASSERT_STREQ("fuga", GS(car(c)));

  Object* cc = cdr(c);
  ASSERT_TRUE(Cell.empty(cc));
}
