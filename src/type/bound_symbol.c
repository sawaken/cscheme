#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

static Object* new(Object* meta, Object* env, Object* symbol)
{
  Data* data = malloc(sizeof(Data));
  data->env = env;
  data->symbol = symbol;

  return CSCM_MetaObject.gen(meta, Type, data);
}

static Object* env(Object* bound_sym)
{
  return PULL(bound_sym)->env;
}

static Object* symbol(Object* bound_sym)
{
  if (PULL(bound_sym)->symbol->type == Type)
    return symbol(PULL(bound_sym)->symbol);
  else
    return PULL(bound_sym)->symbol;
}

CSCM_BoundSymbol_T CSCM_BoundSymbol = {
  {NULL, apply, NULL, NULL},
  .New = new,
  .env = env,
  .symbol = symbol,
};
