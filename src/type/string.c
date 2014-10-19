#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "type.h"

static void* Type = &String;

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

static Object* newWithRange(Object* meta, const char str[], int s, int t)
{
  Data* data = malloc(sizeof(Data));
  data->str = malloc((t - s + 1) * sizeof(char));
  data->length = t - s;

  for (int i = s; i < t; i++) 
    data->str[i - s] = str[i];
  data->str[t - s] = '\0';

  return MetaObject.gen(meta, Type, data);
}

static Object* new(Object* meta, const char string[])
{
  return newWithRange(meta, string, 0, strlen(string));
}

static char* to_s(Object* obj)
{
  return pull(obj)->str;
}

t_String String = {
  {release, NULL, NULL, NULL},
  new, newWithRange, to_s
};
