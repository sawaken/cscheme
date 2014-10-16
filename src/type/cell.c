#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &Cell;

typedef struct
{
  Object* car;
  Object* cdr;
} Data;


static bool empty(Object* obj)
{
  return pull(obj)->car == NULL;
}

static Object* car(Object* obj)
{
  assert(!empty(obj));
  return pull(obj)->car;
}

static Object* cdr(Object* obj)
{
  assert(!empty(obj));
  return pull(obj)->cdr;
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
  assert((car != NULL && cdr != NULL) || (car == NULL && cdr == NULL));

  Data* data = malloc(sizeof(Data));
  data->car = car;
  data->cdr = cdr;
  return MetaObject.gen(meta, Type, data);
}

t_Cell Cell = {
  {NULL, apply, NULL, NULL},
  new, car, cdr, empty
};
