#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &Integer;

typedef struct
{
  int i;
} Data;

static Data* pull(Object* obj)
{
  assert(obj->type == Type);
  return (Data*)(obj->data);
}

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

t_Integer Integer = {
  {NULL, NULL, NULL, NULL},
  new, to_i
};
