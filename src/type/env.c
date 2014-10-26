#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "type.h"

#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
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
  if (PULL(obj)->parent != NULL) 
    proc(PULL(obj)->parent);

  for (int i = 0; i < PULL(obj)->size; i++) {
    proc(PULL(obj)->key[i]);
    proc(PULL(obj)->value[i]);
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
  return PULL(env)->size;
}

static void bind(Object* env, Object* key, Object* value)
{
  int pos = PULL(env)->size++;
  assert(pos < PULL(env)->max_size);
  PULL(env)->key[pos] = key;
  PULL(env)->value[pos] = value;
  CSCM_MetaObject.referred(key);
  CSCM_MetaObject.referred(value);
}

static Object* find(Object* env, Object* key,
		    int (*comp)(Object*, Object*))
{
  for (int i = 0; i < size(env); i++)
    if (comp(PULL(env)->key[i], key) == 0)
      return PULL(env)->value[i];

  if (PULL(env)->parent != NULL) {
    return find(PULL(env)->parent, key, comp);
  } else {
    return NULL;
  }
}

CSCM_Env_T CSCM_Env = {
  {NULL, apply, NULL, NULL},
  new, size, bind, find
};
