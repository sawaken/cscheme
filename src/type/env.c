#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "type.h"

static void* Type = &CSCM_Env;

typedef struct
{
  int size, max_size;
  Object* parent;
  Object* key[10];
  Object* value[10];
} Data;

static void apply(Object* obj, void (*proc)(Object*))
{
  if (pull(obj)->parent != NULL) 
    proc(pull(obj)->parent);

  for (int i = 0; i < pull(obj)->size; i++) {
    proc(pull(obj)->key[i]);
    proc(pull(obj)->value[i]);
  }
}

static Object* new(Object* meta, Object* parent)
{
  Data* data = malloc(sizeof(Data));
  data->parent = parent;
  data->size = 0;
  data->max_size = 10;
  return CSCM_MetaObject.gen(meta, Type, data);
}

static int size(Object* env)
{
  return pull(env)->size;
}

static void bind(Object* env, Object* key, Object* value)
{
  int pos = pull(env)->size++;
  assert(pos < pull(env)->max_size);
  pull(env)->key[pos] = key;
  pull(env)->value[pos] = value;
  CSCM_MetaObject.referred(key);
  CSCM_MetaObject.referred(value);
}

static Object* find(Object* env, Object* key,
		    int (*comp)(Object*, Object*))
{
  for (int i = 0; i < size(env); i++)
    if (comp(pull(env)->key[i], key) == 0)
      return pull(env)->value[i];

  if (pull(env)->parent != NULL) {
    return find(pull(env)->parent, key, comp);
  } else {
    return NULL;
  }
}

CSCM_Env_T CSCM_Env = {
  {NULL, apply, NULL, NULL},
  new, size, bind, find
};
