#include <stdlib.h>
#include <assert.h>
#include "type.h"

static void* Type = &Character;

typedef struct
{
  char c;
} Data;

static Object* new(Object* meta, char c)
{
  Data* data = malloc(sizeof(Data));
  data->c = c;
  return MetaObject.gen(meta, Type, data);
}

static char to_c(Object* obj)
{
  return pull(obj)->c;
}

t_Character Character = {
  {NULL, NULL, NULL, NULL},
  new, to_c
};
