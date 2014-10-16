#include <stdlib.h>
#include <assert.h>
#include "type.h"

static void* Type = &PrimFunc;

typedef struct
{
  char* name;
  Object* (*func)(Object*, Object**, int);
} Data;

static bool release(Object* pf)
{
  free(pull(pf)->name);
  return true;
}

static Object* new(Object* meta, const char name[],
		   Object* (*func)(Object*, Object**, int))
{
  Data data = malloc(sizeof(Data));
  data->name = malloc((strlen(name) + 1) * sizeof(char));
  strcpy(data->name, name);
  data->func = func;

  return MetaObject.gen(meta, Type, data);
}

static Object* apply(Object* pf, Object* meta,
		     Object** args, int argc)
{
  return pull(pf)->func(meta, args, argc);
}

t_PrimFunc PrimFunc = {
  {release, NULL, NULL, NULL},
  new, apply
};
