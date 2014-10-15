#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "type.h"

static void referred(Object*);
static void release(Object*);

static void* Type = &MetaObject;

typedef  struct
{
  int ref_count;
  int unref_count;
  int death_flag;
  int reachable_flag;
} GCInfo;

typedef struct
{
  int size, pos, max_size;
  Object** objects;
  GCInfo** gc_infos;
} Data;


static GCInfo* gc_info(Object* obj)
{
  return pull(obj->meta_obj)->gc_infos[obj->meta_obj_pos];
}

static void add(Object* meta, Object* obj)
{

  if (pull(meta)->pos == pull(meta)->max_size) {
    int k = 0;
    for (int i = 0; i < pull(meta)->pos; i++) {
      if (pull(meta)->objects[i] == NULL) continue;
      pull(meta)->objects[k++] = pull(meta)->objects[i];
    }
    pull(meta)->pos = pull(meta)->size = k;
  }

  int pos = pull(meta)->pos++;
  pull(meta)->objects[pos] = obj;
  obj->meta_obj     = meta;
  obj->meta_obj_pos = pos;
  pull(meta)->gc_infos[pos] = malloc(sizeof(GCInfo));
  memset(pull(meta)->gc_infos[pos], 0, sizeof(GCInfo));
  pull(meta)->size++;
}


/*
  public methods
*/

static Object* new(int init_max_size)
{
  Data* data = malloc(sizeof(Data));
  data->max_size = init_max_size;
  data->objects  = malloc(init_max_size * sizeof(Object*));
  data->gc_infos = malloc(init_max_size * sizeof(GCInfo*));
  data->size = data->pos = 0;

  Object* self = malloc(sizeof(Object));
  self->meta_obj = NULL;
  self->type     = Type;
  self->data     = data;

  return self;
}

static Object* gen(Object* meta, void* type, void* data)
{
  Object* object;

  if (pull(meta)->size == pull(meta)->max_size) {
    return NULL;
  }

  if ((object = malloc(sizeof(Object))) == NULL) {
    return NULL;
  }

  object->type = type;
  object->data = data;
  add(meta, object);

  if (Con(object)->apply != NULL) {
    Con(object)->apply(object, referred);
  }
  return object;
}

static void referred(Object* obj)
{
  Controller* c = Con(obj);

  if (c->onReferred != NULL) {
    c->onReferred(obj);
  }

  gc_info(obj)->ref_count++;
}

static void unreferred(Object* obj)
{
  Controller* c = Con(obj);

  if (c->onUnreferred != NULL) {
    c->onUnreferred(obj);
  }

  gc_info(obj)->unref_count++;

  if (gc_info(obj)->ref_count == gc_info(obj)->unref_count) {
    release(obj);
  }
}

static void release(Object* obj)
{
  Controller* c = Con(obj);
  
  if (gc_info(obj)->death_flag)
    return;

  gc_info(obj)->death_flag = 1;

  if (c->apply != NULL) {
    c->apply(obj, unreferred);
  }

  if (c->release == NULL || c->release(obj)) {
    free(obj->data);
    free(obj);
  }

  free(pull(obj->meta_obj)->gc_infos[obj->meta_obj_pos]);
  pull(obj->meta_obj)->objects[obj->meta_obj_pos] = NULL;
  pull(obj->meta_obj)->size--;
}

static int sweep(Object* obj)
{
  
  return 0;
}


t_MetaObject MetaObject = {
  {NULL, NULL, NULL, NULL},
  new, gen, referred, unreferred, release, sweep
};
