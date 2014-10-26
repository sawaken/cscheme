#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "type.h"
#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))

static void* Type = &CSCM_Cell;

typedef struct
{
  Object* car;
  Object* cdr;
} Data;


static bool empty(Object* obj)
{
  return PULL(obj)->car == NULL;
}

static Object* car(Object* obj)
{
  assert(!empty(obj));
  return PULL(obj)->car;
}

static Object* cdr(Object* obj)
{
  assert(!empty(obj));
  return PULL(obj)->cdr;
}

static void apply(Object* obj, void (*proc)(Object*))
{
  if (!empty(obj)) {
    proc(car(obj));
    proc(cdr(obj));
  }
}

static Object* new(Object* meta, Object* car, Object* cdr)
{
  assert(!(car == NULL && cdr != NULL));

  Data* data = malloc(sizeof(Data));
  data->car = car;

  if (car != NULL && cdr == NULL) {
    data->cdr = new(meta, NULL, NULL);
  } else {
    data->cdr = cdr;
  }

  return CSCM_MetaObject.gen(meta, Type, data);
}

CSCM_Cell_T CSCM_Cell = {
  {NULL, apply, NULL, NULL},
  new, car, cdr, empty
};
