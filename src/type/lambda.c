#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "type.h"

#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
static void* Type = &CSCM_Lambda;

typedef struct
{
  Object* env;
  Object* param;
  Object** exps;
  int expc;
} Data;

static bool release(Object* lambda)
{
  free(PULL(lambda)->exps);
  return true;
}

static void apply(Object* lambda, void (*proc)(Object*))
{
  proc(PULL(lambda)->param);
  for (int i = 0; i < PULL(lambda)->expc; i++)
    proc(PULL(lambda)->exps[i]);
}

static Object* new(Object* meta, Object* env, Object* param, 
		   Object* const exps[], int length)
{
  Data* data = malloc(sizeof(Data));
  data->env   = env;
  data->param = param;
  data->expc  = length;
  data->exps  = malloc(length * sizeof(Object*));

  for (int i = 0; i < length; i++) {
    data->exps[i] = exps[i];
  }

  return CSCM_MetaObject.gen(meta, Type, data);
}

static Object* env(Object* lambda)
{
  return PULL(lambda)->env;
}

static Object* param(Object* lambda)
{
  return PULL(lambda)->param;
}

static Object** exps(Object* lambda)
{
  return PULL(lambda)->exps;
}

static int expc(Object* lambda)
{
  return PULL(lambda)->expc;
}

CSCM_Lambda_T CSCM_Lambda = {
  {release, apply, NULL, NULL},
  new, env, param, exps, expc
};
