#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "type.h"

const void* Type = &String;

typedef struct
{
  int length;
  char* str;
} Data;

static bool release(Object* obj)
{
  free(pull(obj)->str);
  return true;
}

static Object* new(Object* meta, char string[])
{
  Data* data = malloc(sizeof(Data));
  data->length = strlen(string);
  data->str    = malloc((data->length + 1) * sizeof(char));
  strcpy(data->str, string);
  return MetaObject.gen(meta, Type, data);
}

static char* to_s(Object* obj)
{
  return pull(obj)->str;
}

t_String String = {
  {release, NULL, NULL, NULL},
  new, to_s
};
