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

static Object* new(Object* meta,
		   Object* param, 
		   Object** exps, int expc)
{
  Data* data = malloc(sizeof(Data));
  data->param = param;
  data->exps  = exps;
  data->expc  = expc;

  return MetaObject.gen(meta, Type, data);
}

t_Lambda Lambda = {
  {release, apply, NULL, NULL},
  new
};
