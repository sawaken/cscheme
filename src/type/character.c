#include <stdlib.h>
#include <assert.h>
#include "type.h"

static void* Type = &CSCM_Character;

typedef struct
{
  char c;
} Data;

static Object* new(Object* meta, char c)
{
  Data* data = malloc(sizeof(Data));
  data->c = c;
  return CSCM_MetaObject.gen(meta, Type, data);
}

static char to_c(Object* obj)
{
  return pull(obj)->c;
}

CSCM_Character_T CSCM_Character = {
  {NULL, NULL, NULL, NULL},
  new, to_c
};
