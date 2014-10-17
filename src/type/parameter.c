#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "type.h"

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

static Object* new(Object* meta, Object* const params[],
		   int paramc, Object* rest)

{
  Data* data = malloc(sizeof(Data));
  data->params = malloc(paramc * sizeof(Object*));
  data->paramc = paramc;
  data->rest   = rest;

  for (int i = 0; i < paramc; i++) {
    data->params[i] = params[i];
  }

  return MetaObject.gen(meta, Type, data);
}

static int paramc(Object* param)
{
  return pull(param)->paramc;
}

static Object** params(Object* param)
{
  return pull(param)->params;
}

static Object* rest(Object* param)
{
  return pull(param)->rest;
}

static Object* at(Object* param, int pos)
{
  return pull(param)->params[pos];
}

static bool validArgLength(Object* param, int length)
{
  if (rest(param) == NULL)
    return length == paramc(param);
  else
    return length >= paramc(param);
}

t_Parameter Parameter = {
  {release, apply, NULL, NULL},
  new, paramc, params, rest, at, validArgLength
};
