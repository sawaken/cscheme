#include <gtest/gtest.h>
#include <type/type.h>
#include <type/type_alias.h>
#include <iostream>

TEST(take, case1)
{
  Object* meta = MetaObject.New(10);
  Object* dummy = Dummy.New(meta, NULL);
  Object* ex = Exception.New(meta, dummy);

  ASSERT_EQ(dummy, Exception.take(ex));

  MetaObject.release(ex);
  
  ASSERT_TRUE(Dummy.isReleased(dummy));
}
