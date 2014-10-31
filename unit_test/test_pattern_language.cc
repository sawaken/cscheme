#include <gtest/gtest.h>
#include <type/type.h>
#include <type/type_alias.h>
#include <util.h>
#include <pattern_language.h>

class PLTest : public ::testing::Test {
protected:

  Object* meta;

  virtual void SetUp() {
    meta = MetaObject.New(100);
  }
};

TEST_F(PLTest, extractSymbols_case1)
{
  Object* ex  = Util.symList(meta, Util.singletonSymbol, 2, "a", "c");
  Object* ls1 = Util.symList(meta, Util.singletonSymbol, 5, "a", "b", "b", "c", "...");

  Object* buf[100];
  int extracted_num = CSCM_PL.extractSymbols(ex, ls1, buf, 0, 100);

  ASSERT_EQ(1, extracted_num);
  ASSERT_EQ(Util.ith(ls1, 1), buf[0]);
 }


TEST_F(PLTest, extractSymbols_case2)
{
  Object* ex  = Cell.New(meta, NULL, NULL);
  Object* ls1 = Util.symList(meta, Util.singletonSymbol, 3, "a", "b", "c");
  Object* ls2 = Util.symList(meta, Util.singletonSymbol, 3, "b", "c", "d");
  Object* ls3 = Util.list(meta, 2, ls1, ls2);

  Object* buf[100];
  int extracted_num = CSCM_PL.extractSymbols(ex, ls3, buf, 0, 100);

  ASSERT_EQ(4, extracted_num);
  ASSERT_EQ(Util.ith(ls1, 0), buf[0]);
  ASSERT_EQ(Util.ith(ls1, 1), buf[1]);
  ASSERT_EQ(Util.ith(ls1, 2), buf[2]);
  ASSERT_EQ(Util.ith(ls2, 2), buf[3]);
 }
