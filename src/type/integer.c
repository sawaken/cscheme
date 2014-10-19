#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &Integer;

typedef struct
{
  int i;
} Data;

static Object* new(Object* meta, int i)
{
  Data* data = malloc(sizeof(Data));
  data->i = i;
  return MetaObject.gen(meta, Type, data);
}

static int to_i(Object* obj)
{
  return pull(obj)->i;
}

static Object* sum(Object* meta, Object** ints, int len)
{
  int sum = 0;
  for (int i = 0; i < len; i++)
    sum += pull(ints[i])->i;
  return new(meta, sum);
}

t_Integer Integer = {
  {NULL, NULL, NULL, NULL},
  new, to_i, sum
};
