#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &Continuation;

typedef struct
{
  Object** elements;
  Object** evaluated;
  int n, pos;
} Data;


static Data* pull(Object* obj)
{
  assert(obj->type == Type);
  return (Data*)(obj->data);
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

static Object* new(Object* exp)
{
  Data* data = malloc(sizeof(Data));
  int len = Cell.length(exp);
  data->elements  = malloc(len * sizeof(Object*));
  data->evaluated = malloc(len * sizeof(Object*));
  data->n = len, data->pos = 0;

  Object* e = exp;
  for (int i = 0; i < len; i++) {
    data->elements[i] = Cell.car(e);
    e = Cell.cdr(e);
  }

  return New(Type, data);
}


t_Continuation Continuation = {
  {release, referred, unreferred},
  new
};
