#include <stdlib.h>
#include <assert.h>
#include "type.h"

static void release(Object*);

static void* Type = &MetaObject;

typedef struct
{
  int size, max_size;
  Object** objects;
} Data;

static Data* pull(Object* obj)
{
  assert(obj->type == Type);
  return (Data*)(obj->data);
}


static void referred(Object* obj)
{
  Controller* c = Con(obj);

  if (c->referred != NULL) {
    c->referred(obj);
  }

  obj->gc_info.ref_count++;
}

static void unreferred(Object* obj)
{
  Controller* c = Con(obj);

  if (c->unreferred != NULL) {
    c->unreferred(obj);
  }

  obj->gc_info.unref_count++;

  if (obj->gc_info.ref_count == obj->gc_info.unref_count) {
    release(obj);
  }
}

static void release(Object* obj)
{
  Controller* c = Con(obj);
  
  if (obj->gc_info.death_flag) {
    return;
  } else {
    obj->gc_info.death_flag = 1;
  }

  if (c->apply != NULL) {
    c->apply(obj, unreferred);
  }

  if (c->release != NULL) {
    c->release(obj);
  }

  release(obj->data);
  release(obj);
}

static void add(Object* meta, Object* obj)
{
  if (pull(meta)->size == pull(meta)->max_size) {
    
  }
  pull(meta)->objects[pull(meta)->size++] = obj;
}

static Object* gen(Object* meta, void* type, void* data)
{
  Object* object = malloc(sizeof(Object));
  object->meta_obj = meta;
  object->type     = type;
  object->data     = data;
  
  add(meta, object);
  if (Con(object)->apply != NULL) {
    Con(object)->apply(object, referred);
  }
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
};
