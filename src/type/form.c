#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &Form;

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
  free(pull(form)->elements);
  free(pull(form)->evaluated);
  return true;
}

static void apply(Object* form, void (*proc)(Object*))
{
  int size = pull(form)->size, position = pos(form);

  proc(pull(form)->env);

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
    data->elements[i] = Cell.car(e);
    e = Cell.cdr(e);
  }

  return MetaObject.gen(meta, Type, data);
}

static int pos(Object* form)
{
  return pull(form)->pos;
}

static int restNum(Object* form)
{
  return pull(form)->size - pull(form)->pos;
}

static Object* next(Object* form)
{
  return pull(form)->raw_elements[pull(form)->pos];
}

static Object* evaluatedElement(Object* form, int position)
{
  assert(position < pos(form));
  return pull(form)->evaluated_elements[position];
}

static Object* rawElement(Object* form, int position)
{
  assert(position >= pos(form));
  return pull(form)->raw_elements[position];
}

static Object** rawElements(Object* form, int start_pos)
{
  return pull(form)->raw_elements + start_pos;
}

static void back(Object* form, Object* obj)
{
  int position = pull(form)->pos++;

  assert(position < pull(form)->size);
  pull(form)->evaluated_elements[position] = obj;
  MetaObject.referred(obj);
  MetaObject.unreferred(pull(form)->raw_elements[position]);
}

static bool isBody(Object* form)
{
  return pull(form)->body;
}

t_Form Form = {
  {release, apply, NULL, NULL},
  new, pos, restNum, next,
  evaluatedElement, rawElement, rawElements,
  back, isBody
};
