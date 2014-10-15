#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

static void* Type = &Parameter;

typedef struct
{
  int paramc;
  Object** params;
  Object* rest;
} Data;

static bool release(Object* param)
{
  free(pull(param)->params);
  return true;
}

static void apply(Object* param, void (*proc)(Object*))
{
  for (int i = 0; i < pull(param)->paramc; i++)
    proc(pull(param)->params[i]);

  if (pull(param)->rest != NULL)
    proc(pull(param)->rest);
}

static Object* new(Object* meta, Object** params,
		   int paramc, Object* rest)

{
  Data* data = malloc(sizeof(Data));
  data->paramc = paramc;
  data->params = params;
  data->rest   = rest;

  return MetaObject.gen(meta, Type, data);
}


t_Parameter Parameter = {
  {release, apply, NULL, NULL},
  new
};
