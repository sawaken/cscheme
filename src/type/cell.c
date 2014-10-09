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


static Data* pull(Object* obj)
{
  assert(IsA(obj, Type));
  return (Data*)(obj->data);
}

static Object* car(Object* obj)
{
  return pull(obj)->car;
}

static Object* cdr(Object* obj)
{
  return pull(obj)->cdr;
}

static void apply(Object* obj, int (*proc)(Object*))
{
  proc(car(obj));
  proc(cdr(obj));
}

static void release(Object* obj)
{
 
}

static void referred(Object* obj)
{

}

static void unreferred(Object* obj)
{

}

static Object* new(Object* meta, Object* car, Object* cdr)
{
  Data* data = malloc(sizeof(Data));
  data->car = car;
  data->cdr = cdr;
  return MetaObject.gen(meta, Type, data);
}

t_Cell Cell = {
  {NULL, apply, NULL, NULL},
  new, car, cdr
};
