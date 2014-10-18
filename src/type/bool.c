#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "type.h"

static void* Type = &Bool;

typedef struct
{
  bool b;
} Data;

static Object* new(Object* meta, bool b)
{
  Data* data = malloc(sizeof(Data));
  data->b = b;
  return MetaObject.gen(meta, Type, data);
}

static bool to_b(Object* b)
 {
   return pull(b)->b;
 }

static Object* select(Object* b, Object* obj1, Object* obj2)
{
  if (IsA(b, &Bool) && to_b(b)) return obj1;
  return obj2;
}

t_Bool Bool = {
  {NULL, NULL, NULL, NULL},
  new, to_b, select
};
