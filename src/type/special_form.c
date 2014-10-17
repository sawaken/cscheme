#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &SpecialForm;

typedef struct
{
  char* name;
  bool (*action)(Object* meta, Object* cont);
  int len_min, len_max;
} Data;

static bool release(Object* obj)
{
  free(pull(obj)->name);
  return true;
}

static Object* new(Object* meta, const char name[],
		   bool (*action)(Object* meta, Object* cont),
		   int len_min, int len_max)
{
  Data* data = malloc(sizeof(Data));
  data->name = malloc((strlen(name) + 1) * sizeof(char));
  strcpy(data->name, name);
  data->action = action;
  data->len_min = len_min;
  data->len_max = len_max;
  return MetaObject.gen(meta, Type, data);
}

static bool validArgc(Object* sf, int argc)
{
  return pull(sf)->len_min <= argc && argc <= pull(sf)->len_max;
}

static bool doAction(Object* sf, Object* meta, Object* cont)
{
  return pull(sf)->action(meta, cont);
}

t_SpecialForm SpecialForm = {
  {release, NULL, NULL, NULL},
  new, validArgc, doAction
};
