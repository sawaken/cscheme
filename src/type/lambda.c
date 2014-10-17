#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "type.h"

static void* Type = &Lambda;

typedef struct
{
  Object* env;
  Object* param;
  Object** exps;
  int expc;
} Data;

static bool release(Object* lambda)
{
  free(pull(lambda)->exps);
  return true;
}

static void apply(Object* lambda, void (*proc)(Object*))
{
  proc(pull(lambda)->param);
  for (int i = 0; i < pull(lambda)->expc; i++)
    proc(pull(lambda)->exps[i]);
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

  return MetaObject.gen(meta, Type, data);
}

static Object* env(Object* lambda)
{
  return pull(lambda)->env;
}

static Object* param(Object* lambda)
{
  return pull(lambda)->param;
}

static Object** exps(Object* lambda)
{
  return pull(lambda)->exps;
}

static int expc(Object* lambda)
{
  return pull(lambda)->expc;
}

t_Lambda Lambda = {
  {release, apply, NULL, NULL},
  new, env, param, exps, expc
};
