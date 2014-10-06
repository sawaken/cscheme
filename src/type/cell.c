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
  assert(obj->type == Type);
  return (Data*)(obj->data);
}

static void release(Object* obj)
{
  Object* car = pull(obj)->car;
  Object* cdr = pull(obj)->cdr;
  con(car)->unreferred(car);
  con(cdr)->unreferred(cdr);
}

static void referred(Object* obj)
{

}

static void unreferred(Object* obj)
{

}


static Object* new(Object* car, Object* cdr)
{
  Data* data = malloc(sizeof(Data));
  data->car = car;
  data->cdr = cdr;
  return New(Type, data);
}

t_Cell Cell = {
  release, referred, unreferred,
  new
};
