#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &Integer;

typedef struct
{
  int i;
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

static Object* new(int i)
{
  Data* data = malloc(sizeof(Data));
  strcpy(data->name, name);
  return NewObject(Type, data);
}

static int to_i(Object* obj)
{
  return pull(obj)->i;
}

t_Integer Integer = {
  release, referred, unrefferred,
  new, to_i
};
