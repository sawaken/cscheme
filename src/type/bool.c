#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "type.h"

#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
static void* Type = &CSCM_Bool;

typedef struct
{
  bool b;
} Data;

static Object* new(Object* meta, bool b)
{
  Data* data = malloc(sizeof(Data));
  data->b = b;
  return CSCM_MetaObject.gen(meta, Type, data);
}

static bool to_b(Object* b)
 {
   return PULL(b)->b;
 }

static Object* select(Object* b, Object* obj1, Object* obj2)
{
  if (IsA(b, Type) && !to_b(b)) return obj2;
  return obj1;
}

CSCM_Bool_T CSCM_Bool = {
  {NULL, NULL, NULL, NULL},
  new, to_b, select
};
