#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
static void* Type = &CSCM_SpecialForm;

typedef struct
{
  char* name;
  bool (*action)(Object* meta, Object* cont);
  int len_min, len_max;
} Data;

static bool release(Object* obj)
{
  free(PULL(obj)->name);
  return true;
}

static Object* new(Object* meta, const char name[],
		   bool (*action)(Object* meta, Object* cont))
{
  Data* data = malloc(sizeof(Data));
  data->name = malloc((strlen(name) + 1) * sizeof(char));
  strcpy(data->name, name);
  data->action = action;
  return CSCM_MetaObject.gen(meta, Type, data);
}

static bool doAction(Object* sf, Object* meta, Object* cont)
{
  return PULL(sf)->action(meta, cont);
}

CSCM_SpecialForm_T CSCM_SpecialForm = {
  {release, NULL, NULL, NULL},
  new, doAction
};
