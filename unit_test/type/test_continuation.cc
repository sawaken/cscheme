#include <gtest/gtest.h>
#include <type/type.h>

#define C Continuation
#define D Dummy

class UseTest : public ::testing::Test {
protected:

  Object *meta;
  Object *dummy1, *dummy2, *dummy3;
  Object *dummyA, *dummyB, *dummyC;

  virtual void SetUp() {
    meta = MetaObject.New(100);
    dummy1 = D.New(meta, NULL);
    dummy2 = D.New(meta, NULL);
    dummy3 = D.New(meta, NULL);
    dummyA = D.New(meta, NULL);
    dummyB = D.New(meta, dummyA);
    dummyC = D.New(meta, dummyB);
  }
};

Object* makeForm(Object* meta, Object* e, Object* a, Object* b)
{
  return Form.New(meta, e, Cell.New(meta, a, Cell.New(meta, b, NULL)), 2, false);
}

TEST_F(UseTest, top_size_at_erase_pop_push)
{
  Object* cont = C.New(meta, NULL);

  ASSERT_EQ(0, C.size(cont));

  C.push(cont, dummy1);
  C.push(cont, dummy2);
  C.push(cont, dummy3);

  ASSERT_EQ(3, C.size(cont));
  ASSERT_EQ(dummy3, C.top(cont));
  ASSERT_EQ(dummy1, C.at(cont, 0));
  ASSERT_EQ(dummy2, C.at(cont, 1));
  ASSERT_EQ(dummy3, C.at(cont, 2));

  C.erase(cont, 0, 1);
  C.pop(cont);

  ASSERT_TRUE(D.isReleased(dummy1));
  ASSERT_FALSE(D.isReleased(dummy2));
  ASSERT_TRUE(D.isReleased(dummy3));
}
  
TEST_F(UseTest, popAndPush)
{
  Object* cont = C.New(meta, NULL);

  C.push(cont, dummyC);
  C.popAndPush(cont, dummyA);

  ASSERT_FALSE(D.isReleased(dummyA));
  ASSERT_TRUE(D.isReleased(dummyB));
  ASSERT_TRUE(D.isReleased(dummyC));
}

TEST_F(UseTest, trans)
{
  Object* cont1 = C.New(meta, NULL);
  Object* cont2 = C.New(meta, NULL);

  C.push(cont1, dummy1);
  C.push(cont1, dummy2);
  C.push(cont1, dummy3);

  C.push(cont2, dummy1);
  C.push(cont2, dummy2);

  C.trans(cont1, cont2, dummyA);

  ASSERT_FALSE(D.isReleased(dummy1));
  ASSERT_FALSE(D.isReleased(dummy2));
  ASSERT_TRUE(D.isReleased(dummy3));
}

TEST_F(UseTest, returnTopToForm)
{
  Object* cont = C.New(meta, NULL);

  C.push(cont, makeForm(meta, dummyA, dummy1, dummy2));
  C.push(cont, dummy1);
  C.returnTopToForm(cont);

  ASSERT_FALSE(D.isReleased(dummy1));
}

TEST_F(UseTest, New_with_init_cont)
{
  Object* init_cont = C.New(meta, NULL);

  C.push(init_cont, dummy1);
  C.push(init_cont, dummy2);

  Object* cont = C.New(meta, init_cont);

  ASSERT_EQ(2, C.size(cont));
  ASSERT_EQ(dummy2, C.top(cont));

  ASSERT_EQ(2, D.ref_count(dummy1));
  ASSERT_EQ(2, D.ref_count(dummy2));

  ASSERT_EQ(0, D.unref_count(dummy1));
  ASSERT_EQ(0, D.unref_count(dummy2));
}
