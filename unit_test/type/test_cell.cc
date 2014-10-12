#include <gtest/gtest.h>
#include <type/type.h>
#include <iostream>


TEST(car_cdr, case1)
{
  Object* meta = MetaObject.New(10);
  Object* dum1 = Dummy.New(meta, NULL);
  Object* dum2 = Dummy.New(meta, NULL);
  Object* cell = Cell.New(meta, dum1, dum2);

  ASSERT_EQ(dum1, Cell.car(cell));
  ASSERT_EQ(1, Dummy.ref_count(dum1));

  ASSERT_EQ(dum2, Cell.cdr(cell));
  ASSERT_EQ(1, Dummy.ref_count(dum2));

  MetaObject.release(cell);

  ASSERT_EQ(1, Dummy.unref_count(dum1));
  ASSERT_EQ(true, Dummy.isReleased(dum1));

  ASSERT_EQ(1, Dummy.unref_count(dum2));
  ASSERT_EQ(true, Dummy.isReleased(dum2));  
}


TEST(empty_cell, case1)
{
  Object* meta = MetaObject.New(10);
  Object* ecell = Cell.New(meta, NULL, NULL);
  Object* cell  = Cell.New(meta, ecell, ecell);

  ASSERT_EQ(true, Cell.empty(ecell));
  ASSERT_EQ(false, Cell.empty(cell));
}
