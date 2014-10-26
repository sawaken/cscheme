#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "type.h"

static void* Type = &CSCM_Exception;

typedef struct
{
  Object* raised_obj;
} Data;

static void apply(Object* obj, void (*proc)(Object*))
{
  proc(pull(obj)->raised_obj);
}

static Object* new(Object* meta, Object* raised_obj)
{
  Data* data = malloc(sizeof(Data));
  data->raised_obj = raised_obj;
  return CSCM_MetaObject.gen(meta, Type, data);
}

static Object* take(Object* exception)
{
  return pull(exception)->raised_obj;
}

CSCM_Exception_T CSCM_Exception = {
  {NULL, apply, NULL, NULL},
  new, take
};
