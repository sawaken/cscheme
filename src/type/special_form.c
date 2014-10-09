#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &SpecialForm;

typedef struct
{
  char name[10];
  void (*action)(Object* cont);
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

static Object* new(char* name, void (*action)(Object* cont))
{
  Data* data = malloc(sizeof(Data));
  strcpy(data->name, name);
  data->action = action;
  return New(Type, data);
}

static Object* doAction(Object* sf, Object* cont)
{
  pull(sf)->action(cont);
}

t_SpecialForm SpecialForm = {
  {release, referred, unreferred},
  new, doAction
};
