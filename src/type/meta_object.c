#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "type.h"
#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))

static void referred(Object*);
static void release(Object*);

static void* Type = &CSCM_MetaObject;

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

static Controller* Con(Object* obj)
{
  return (Controller*)(obj->type);
}

static GCInfo* gc_info(Object* obj)
{
  return PULL(obj->meta_obj)->gc_infos[obj->meta_obj_pos];
}

static void add(Object* meta, Object* obj)
{

  if (PULL(meta)->pos == PULL(meta)->max_size) {
    int k = 0;
    for (int i = 0; i < PULL(meta)->pos; i++) {
      if (PULL(meta)->objects[i] == NULL) continue;
      PULL(meta)->objects[k] = PULL(meta)->objects[i];
      PULL(meta)->gc_infos[k] = PULL(meta)->gc_infos[i];
      PULL(meta)->objects[k]->meta_obj_pos = k;
      k++;
    }
    PULL(meta)->pos = k;
    PULL(meta)->size = k;
  }

  int pos = PULL(meta)->pos++;
  PULL(meta)->objects[pos] = obj;
  obj->meta_obj     = meta;
  obj->meta_obj_pos = pos;
  PULL(meta)->gc_infos[pos] = malloc(sizeof(GCInfo));
  memset(PULL(meta)->gc_infos[pos], 0, sizeof(GCInfo));
  PULL(meta)->size++;
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

  if (PULL(meta)->size == PULL(meta)->max_size) {
    printf("meta_object overflow\n");
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

static int size(Object* meta)
{
  return PULL(meta)->size;
}

static int pos(Object* meta)
{
  return PULL(meta)->pos;
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

  Object* meta_obj = obj->meta_obj;
  int meta_obj_pos = obj->meta_obj_pos;

  if (c->release == NULL || c->release(obj)) {
    free(obj->data);
    free(obj);
  }

  free(PULL(meta_obj)->gc_infos[meta_obj_pos]);  
  PULL(meta_obj)->objects[meta_obj_pos] = NULL;
  PULL(meta_obj)->size--;
}

static int sweep(Object* obj)
{
  
  return 0;
}

static Object* findSymbol(Object* meta, const char* name,
			  int (*strcmp)(const char* s1, const char* s2))
{
  Object** os = PULL(meta)->objects;

  for (int i = 0; i < PULL(meta)->pos; i++) {
    if (os[i] == NULL) continue;
    if (IsA(os[i], &CSCM_Symbol) && strcmp(name, CSCM_Symbol.to_s(os[i])) == 0)
      return os[i];
  }
  return NULL;
}

CSCM_MetaObject_T CSCM_MetaObject = {
  {NULL, NULL, NULL, NULL},
  new, gen, size, pos, referred, unreferred, release, sweep, findSymbol
};
