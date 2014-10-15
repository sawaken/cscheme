#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "type.h"

static void* Type = &Env;

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
  return MetaObject.gen(meta, Type, data);
}

static void bind(Object* env, Object* key, Object* value)
{
  int pos = pull(env)->size++;
  assert(pos < pull(env)->max_size);
  pull(env)->key[pos] = key;
  pull(env)->value[pos] = value;
}

static Object* find(Object* env, Object* key,
		    bool (*comp)(Object*, Object*))
{
  for (int i = 0; i < size; i++)
    if (comp(pull(env)->key[i], key))
      return pull(env)->value[i];

  if (pull(env)->parent != NULL) {
    return Env.find(pull(env)->parent, key, comp);
  } else {
    return NULL;
  }
}

t_Env Env = {
  {NULL, apply, NULL, NULL},
  new, bind, find
};
