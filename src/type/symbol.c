#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &Symbol;

typedef struct
{
  char name[100];
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

static Object* new(char* name)
{
  Data* data = malloc(sizeof(Data));
  strcpy(data->name, name);
  return New(Type, data);
}

static char* to_s(Object* obj)
{
  return pull(obj)->name;
}


t_Symbol Symbol = {
  release, referred, unreferred,
  new, to_s
};
