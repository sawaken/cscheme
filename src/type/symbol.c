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

static Object* newWithRange(Object* meta, const char str[], int s, int t)
{
  Data* data = malloc(sizeof(Data));
  data->name = malloc((t - s + 1) * sizeof(char));

  for (int i = s; i < t; i++) 
    data->name[i - s] = str[i];
  data->name[t - s] = '\0';

  return MetaObject.gen(meta, Type, data);
}

static Object* new(Object* meta, const char name[])
{
  return newWithRange(meta, name, 0, strlen(name));
}

static char* to_s(Object* obj)
{
  return pull(obj)->name;
}

t_Symbol Symbol = {
  {release, NULL, NULL, NULL},
  new, newWithRange, to_s
};
