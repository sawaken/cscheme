#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
#include "type.h"

static void* Type = &CSCM_Form;

static int pos(Object* form);
static Object* evaluatedElement(Object* form, int position);
static Object* rawElement(Object* form, int position);

typedef struct
{
  Object* env;
  Object** raw_elements;
  Object** evaluated_elements;
  int size, pos;
  bool body;
} Data;

static bool release(Object* form)
{
  free(PULL(form)->raw_elements);
  free(PULL(form)->evaluated_elements);
  return true;
}

static void apply(Object* form, void (*proc)(Object*))
{
  int size = PULL(form)->size, position = pos(form);

  proc(PULL(form)->env);

  for (int i = 0; i < size; i++) {
    if (i < position)
      proc(evaluatedElement(form, i));
    else
      proc(rawElement(form, i));
  }
}

static Object* new(Object* meta, Object* env, Object* exp,
		   int length, bool body)
{
  Data* data = malloc(sizeof(Data));
  data->env = env;
  data->raw_elements       = malloc(length * sizeof(Object*));
  data->evaluated_elements = malloc(length * sizeof(Object*));
  data->size = length, data->pos = 0, data->body = body;

  Object* e = exp;
  for (int i = 0; i < length; i++) {
    data->raw_elements[i] = CSCM_Cell.car(e);
    e = CSCM_Cell.cdr(e);
  }

  return CSCM_MetaObject.gen(meta, Type, data);
}

static int pos(Object* form)
{
  return PULL(form)->pos;
}

static int restNum(Object* form)
{
  return PULL(form)->size - PULL(form)->pos;
}

static int size(Object* form)
{
  return PULL(form)->size;
}

static Object* env(Object* form)
{
  return PULL(form)->env;
}

static Object* next(Object* form)
{
  assert(pos(form) < PULL(form)->size);
  return PULL(form)->raw_elements[PULL(form)->pos];
}

static Object* evaluatedElement(Object* form, int position)
{
  assert(position < pos(form));
  return PULL(form)->evaluated_elements[position];
}

static Object* rawElement(Object* form, int position)
{
  assert(position >= pos(form));
  return PULL(form)->raw_elements[position];
}

static Object** evaluatedElements(Object* form, int start_pos)
{
  return PULL(form)->evaluated_elements + start_pos;
}

static Object** rawElements(Object* form, int start_pos)
{
  return PULL(form)->raw_elements + start_pos;
}

static void back(Object* form, Object* obj)
{
  int position = PULL(form)->pos++;

  assert(position < PULL(form)->size);
  PULL(form)->evaluated_elements[position] = obj;
  CSCM_MetaObject.referred(obj);
  CSCM_MetaObject.unreferred(PULL(form)->raw_elements[position]);
}

static bool isBody(Object* form)
{
  return PULL(form)->body;
}

CSCM_Form_T CSCM_Form = {
  {release, apply, NULL, NULL},
  new, pos, restNum, size, env, next,
  evaluatedElement, rawElement, evaluatedElements, rawElements,
  back, isBody
};
