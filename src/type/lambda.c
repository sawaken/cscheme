#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

static void* Type = &Lambda;

typedef struct
{
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

static Object* new(Object* meta, Object* param, 
		   const Object* exps[], int length)
{
  Data* data = malloc(sizeof(Data));
  data->param = param;
  data->expc  = length;
  data->exps  = malloc(length * sizeof(Object*));

  for (int i = 0; i < length; i++) {
    data->exps[i] = exps[i]
  }

  return MetaObject.gen(meta, Type, data);
}

static Object* param(Object* lambda)
{
  pull(lambda)->param;
}

static Object** exps(Object* lambda)
{
  pull(lambda)->exps;
}

static int expc(Object* lambda)
{
  pull(lambda)->expc;
}

t_Lambda Lambda = {
  {release, apply, NULL, NULL},
  new, param, exps, expc
};
