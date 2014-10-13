#include <gtest/gtest.h>
#include <type/type.h>
#include <list_util.h>
#include <iostream>

class UseTest : public ::testing::Test {
protected:

  Generator g;
  Object *meta, *exp, *form;
  Object *a, *b, *c, *x, *y, *z, *env;
  
  virtual void SetUp() {
    meta = MetaObject.New(100);
    g = (Generator){meta, Cell.New, NULL, NULL};
    a = Dummy.New(meta, NULL);
    b = Dummy.New(meta, NULL);
    c = Dummy.New(meta, NULL);
    x = Dummy.New(meta, NULL);
    y = Dummy.New(meta, NULL);
    z = Dummy.New(meta, NULL);
    env = Dummy.New(meta, NULL);
    exp  = LS.New(&g, 3, a, b, c);
    form = Form.New(meta, env, exp, 3, true);
  }
};

TEST_F(UseTest, phase0)
{
  ASSERT_TRUE(Form.isBody(form));

  ASSERT_EQ(2, Dummy.ref_count(a));
  ASSERT_EQ(0, Dummy.unref_count(a));

  ASSERT_EQ(2, Dummy.ref_count(b));
  ASSERT_EQ(0, Dummy.unref_count(b));

  ASSERT_EQ(2, Dummy.ref_count(c));
  ASSERT_EQ(0, Dummy.unref_count(c));

  ASSERT_EQ(0, Dummy.ref_count(x));
  ASSERT_EQ(0, Dummy.unref_count(x));

  ASSERT_EQ(0, Dummy.ref_count(y));
  ASSERT_EQ(0, Dummy.unref_count(y));

  ASSERT_EQ(0, Dummy.ref_count(z));
  ASSERT_EQ(0, Dummy.unref_count(z));

  ASSERT_EQ(1, Dummy.ref_count(env));
  ASSERT_EQ(0, Dummy.unref_count(env));
}

TEST_F(UseTest, phase1)
{
  MetaObject.release(exp);

  ASSERT_FALSE(Dummy.isReleased(a));
  ASSERT_FALSE(Dummy.isReleased(b));
  ASSERT_FALSE(Dummy.isReleased(c));
  ASSERT_FALSE(Dummy.isReleased(env));

  ASSERT_EQ(0, Form.pos(form));
  ASSERT_EQ(3, Form.restNum(form));
  ASSERT_EQ(a, Form.next(form));

  ASSERT_EQ(a, Form.rawElement(form, 0));
  ASSERT_EQ(b, Form.rawElement(form, 1));
  ASSERT_EQ(c, Form.rawElement(form, 2));

  ASSERT_EQ(a, *(Form.rawElements(form, 0) + 0));
  ASSERT_EQ(b, *(Form.rawElements(form, 0) + 1));
  ASSERT_EQ(c, *(Form.rawElements(form, 0) + 2));

  ASSERT_EQ(b, *(Form.rawElements(form, 1) + 0));
  ASSERT_EQ(c, *(Form.rawElements(form, 1) + 1));

  ASSERT_EQ(c, *(Form.rawElements(form, 2) + 0));
}

TEST_F(UseTest, phase2)
{
  MetaObject.release(exp);
  Form.back(form, x);
  
  ASSERT_EQ(1, Dummy.ref_count(x));
  ASSERT_TRUE(Dummy.isReleased(a));
  ASSERT_FALSE(Dummy.isReleased(b));
  ASSERT_FALSE(Dummy.isReleased(c));
  ASSERT_FALSE(Dummy.isReleased(env));
  
  ASSERT_EQ(1, Form.pos(form));
  ASSERT_EQ(2, Form.restNum(form));
  ASSERT_EQ(b, Form.next(form));

  ASSERT_EQ(x, Form.evaluatedElement(form, 0));
  ASSERT_EQ(b, Form.rawElement(form, 1));
  ASSERT_EQ(c, Form.rawElement(form, 2));

  ASSERT_EQ(b, *(Form.rawElements(form, 1) + 0));
  ASSERT_EQ(c, *(Form.rawElements(form, 1) + 1));

  ASSERT_EQ(c, *(Form.rawElements(form, 2) + 0));
}

TEST_F(UseTest, phase3)
{
  MetaObject.release(exp);
  Form.back(form, x);
  Form.back(form, y);

  ASSERT_EQ(1, Dummy.ref_count(x));
  ASSERT_EQ(1, Dummy.ref_count(y));
  ASSERT_TRUE(Dummy.isReleased(a));
  ASSERT_TRUE(Dummy.isReleased(b));
  ASSERT_FALSE(Dummy.isReleased(c));
  ASSERT_FALSE(Dummy.isReleased(env));
  
  ASSERT_EQ(2, Form.pos(form));
  ASSERT_EQ(1, Form.restNum(form));
  ASSERT_EQ(c, Form.next(form));

  ASSERT_EQ(x, Form.evaluatedElement(form, 0));
  ASSERT_EQ(y, Form.evaluatedElement(form, 1));
  ASSERT_EQ(c, Form.rawElement(form, 2));

  ASSERT_EQ(c, *(Form.rawElements(form, 2) + 0));
}

TEST_F(UseTest, last_phase)
{
  MetaObject.release(exp);
  Form.back(form, x);
  Form.back(form, y);
  Form.back(form, z);

  ASSERT_EQ(1, Dummy.ref_count(x));
  ASSERT_EQ(1, Dummy.ref_count(y));
  ASSERT_EQ(1, Dummy.ref_count(z));
  ASSERT_TRUE(Dummy.isReleased(a));
  ASSERT_TRUE(Dummy.isReleased(b));
  ASSERT_TRUE(Dummy.isReleased(c));
  ASSERT_FALSE(Dummy.isReleased(env));
  
  ASSERT_EQ(3, Form.pos(form));
  ASSERT_EQ(0, Form.restNum(form));

  ASSERT_EQ(x, Form.evaluatedElement(form, 0));
  ASSERT_EQ(y, Form.evaluatedElement(form, 1));
  ASSERT_EQ(z, Form.evaluatedElement(form, 2));
}

TEST_F(UseTest, release_phase)
{
  MetaObject.release(exp);
  Form.back(form, x);
  Form.back(form, y);
  Form.back(form, z);
  MetaObject.release(form);

  ASSERT_TRUE(Dummy.isReleased(x));
  ASSERT_TRUE(Dummy.isReleased(y));
  ASSERT_TRUE(Dummy.isReleased(z));
  ASSERT_TRUE(Dummy.isReleased(env));
}
