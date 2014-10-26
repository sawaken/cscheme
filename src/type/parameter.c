#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "type.h"

#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
static void* Type = &CSCM_Parameter;

typedef struct
{
  int paramc;
  Object** params;
  Object* rest;
} Data;

static bool release(Object* param)
{
  free(PULL(param)->params);
  return true;
}

static void apply(Object* param, void (*proc)(Object*))
{
  for (int i = 0; i < PULL(param)->paramc; i++)
    proc(PULL(param)->params[i]);

  if (PULL(param)->rest != NULL)
    proc(PULL(param)->rest);
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

  return CSCM_MetaObject.gen(meta, Type, data);
}

static int paramc(Object* param)
{
  return PULL(param)->paramc;
}

static Object** params(Object* param)
{
  return PULL(param)->params;
}

static Object* rest(Object* param)
{
  return PULL(param)->rest;
}

static Object* at(Object* param, int pos)
{
  return PULL(param)->params[pos];
}

static bool validArgLength(Object* param, int length)
{
  if (rest(param) == NULL)
    return length == paramc(param);
  else
    return length >= paramc(param);
}

CSCM_Parameter_T CSCM_Parameter = {
  {release, apply, NULL, NULL},
  new, paramc, params, rest, at, validArgLength
};
