#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "type.h"

static void* Type = &CSCM_Dummy;

typedef struct
{
  Object* ref;
  int ref_count, unref_count;
  bool is_released;
} Data;

static bool release(Object* obj)
{
  pull(obj)->is_released = true;
  return false;
}

static void apply(Object* obj, void (*proc)(Object*))
{
  if (pull(obj)->ref != NULL)
    proc(pull(obj)->ref);
}

static void onReferred(Object* obj)
{
  pull(obj)->ref_count++;
}

static void onUnreferred(Object* obj)
{
  pull(obj)->unref_count++;
}

static Object* new(Object* meta, Object* ref)
{
  Data* data = malloc(sizeof(Data));
  data->ref = ref;
  data->ref_count = data->unref_count = 0;
  data->is_released = false;
  return CSCM_MetaObject.gen(meta, Type, data);
}

static Object* ref(Object* obj)
{
  return pull(obj)->ref;
}

static bool isReleased(Object* obj)
{
  return pull(obj)->is_released;
}

static int ref_count(Object* obj)
{
  return pull(obj)->ref_count;
}

static int unref_count(Object* obj)
{
  return pull(obj)->unref_count;
}

CSCM_Dummy_T CSCM_Dummy = {
  {release, apply, onReferred, onUnreferred},
  new, ref, isReleased, ref_count, unref_count
};
