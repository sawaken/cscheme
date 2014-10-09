#include <stdlib.h>
#include "type.h"

static void* Type = &MetaObject;

typedef struct
{
  int size, max_size;
  Object** objects;
} Data;


static void referred(Object* obj)
{
  Controller* c = Con(obj);

  if (c->referred != NULL) {
    c->referred(obj);
  }

  obj->ref_count++;
}

static void unreferred(Object* obj)
{
  Controller* c = Con(obj);

  if (c->unreferred != NULL) {
    c->unreferred(obj);
  }

  obj->gcd.unref_count++;

  if (obj->gcd.ref_count == obj->gcd.unref_count) {
    Release(obj);
  }
}

static void release(Object* obj)
{
  Controller* c = Con(obj);
  
  if (obj->gcd.death_flag) {
    return 0;
  } else {
    obj->gcd.death_flag = 1;
  }

  if (c->apply != NULL) {
    c->apply(obj, Unreferred);
  }

  if (c->release != NULL) {
    c->release(obj);
  }

  release(obj->data);
  release(obj);
}

static void add(Object* meta, Object* obj)
{
  if (size == max_size) {
    
  }
  objects[size++] = obj;
}

static Object* gen(Object* meta, void* type, void* data)
{
  Object* object = malloc(sizeof(Object));
  object->meta_obj = meta;
  object->type     = type;
  object->data     = data;
  
  add(meta, object);
  Con(object)->apply(object, referred);
  return object;
}

static Object* new(int init_max_size)
{
  Data* data = malloc(sizeof(Data));
  data->max_size = init_max_size;
  data->objects  = malloc(init_max_size * sizeof(Object*));
  data->size     = 0;

  Object* self = malloc(sizeof(Object));
  self->meta_obj = self;
  self->type     = Type;
  self->data     = data;

  return self;
}

t_MetaObject MetaObject = {
  {NULL, NULL, NULL, NULL},
  new, gen
}
