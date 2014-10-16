#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &Symbol;

typedef struct
{
  char* name;
} Data;

static bool release(Object* obj)
{
  free(pull(obj)->name);
  return true;
}

static Object* new(Object* meta, const char name[])
{
  Data* data = malloc(sizeof(Data));
  data->name = malloc((strlen(name) + 1) * sizeof(char));
  strcpy(data->name, name);
  return MetaObject.gen(meta, Type, data);
}

static char* to_s(Object* obj)
{
  return pull(obj)->name;
}


t_Symbol Symbol = {
  {release, NULL, NULL, NULL},
  new, to_s
};
