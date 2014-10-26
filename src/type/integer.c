#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
static void* Type = &CSCM_Integer;

typedef struct
{
  int i;
} Data;

static Object* new(Object* meta, int i)
{
  Data* data = malloc(sizeof(Data));
  data->i = i;
  return CSCM_MetaObject.gen(meta, Type, data);
}

static int to_i(Object* obj)
{
  return PULL(obj)->i;
}

static Object* sum(Object* meta, Object** ints, int len)
{
  int sum = 0;
  for (int i = 0; i < len; i++)
    sum += PULL(ints[i])->i;
  return new(meta, sum);
}

CSCM_Integer_T CSCM_Integer = {
  {NULL, NULL, NULL, NULL},
  new, to_i, sum
};
