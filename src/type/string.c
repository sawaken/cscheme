#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "type.h"
#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))

static void* Type = &CSCM_String;

typedef struct
{
  int length;
  char* str;
} Data;

static bool release(Object* obj)
{
  free(PULL(obj)->str);
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

  return CSCM_MetaObject.gen(meta, Type, data);
}

static Object* new(Object* meta, const char string[])
{
  return newWithRange(meta, string, 0, strlen(string));
}

static char* to_s(Object* obj)
{
  return PULL(obj)->str;
}

CSCM_String_T CSCM_String = {
  {release, NULL, NULL, NULL},
  new, newWithRange, to_s
};
