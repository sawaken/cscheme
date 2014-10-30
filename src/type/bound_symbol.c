#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "type.h"

#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
static void* Type = &CSCM_BoundSymbol;

typedef struct
{
  Object* env;
  Object* symbol;
} Data;

static void apply(Object* obj, void (*proc)(Object*))
{
  proc(PULL(obj)->env);
  proc(PULL(obj)->symbol);
}

static Object* new(Object* meta, Object* env, Object* exp)
{
  Data* data = malloc(sizeof(Data));
  data->env = env;
  data->symbol = symbol;
  return CSCM_MetaObject.gen(meta, Type, data);
}

static Object* env(Object* bound_symbol)
{
  return PULL(bound_symbol)->env;
}

static Object* symbol(Object* bound_symbol)
{
  return PULL(bound_symbol)->symbol;
}

CSCM_BoundSymbol_T CSCM_BoundSymbol = {
  {NULL, apply, NULL, NULL},
  .New = new,
  .env = env,
  .symbol = symbol,
};
