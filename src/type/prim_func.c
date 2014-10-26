#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "type.h"

#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
static void* Type = &CSCM_PrimFunc;

typedef struct
{
  char* name;
  Object* (*func)(Object*, Object**, int);
} Data;

static bool release(Object* pf)
{
  free(PULL(pf)->name);
  return true;
}

static Object* new(Object* meta, const char name[],
		   Object* (*func)(Object*, Object**, int))
{
  Data* data = malloc(sizeof(Data));
  data->name = malloc((strlen(name) + 1) * sizeof(char));
  strcpy(data->name, name);
  data->func = func;

  return CSCM_MetaObject.gen(meta, Type, data);
}

static Object* apply(Object* pf, Object* meta,
		     Object** args, int argc)
{
  return PULL(pf)->func(meta, args, argc);
}

CSCM_PrimFunc_T CSCM_PrimFunc = {
  {release, NULL, NULL, NULL},
  new, apply
};
