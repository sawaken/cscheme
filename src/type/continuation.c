#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
static void* Type = &CSCM_Continuation;

static Object* at(Object* cont, int pos);
static int size(Object* cont);
static int max_size(Object* cont);

typedef struct
{
  int size, max_size;
  Object** stack;
} Data;


static bool release(Object* obj)
{
  free(PULL(obj)->stack);
  return true;
}

static void apply(Object* obj, void (*proc)(Object*))
{
  for (int i = 0; i < size(obj); i++)
    proc(at(obj, i));
}

static Object* new(Object* meta, Object* init_cont)
{
  Data* data = malloc(sizeof(Data));

  if (init_cont == NULL) {
    data->size = 0, data->max_size = 100;
    data->stack = malloc(data->max_size * sizeof(Object*));
  }
  else {
    data->size = size(init_cont);
    data->max_size = max_size(init_cont);
    data->stack = malloc(data->max_size * sizeof(Object*));
    for (int i = 0; i < data->size; i++)
      data->stack[i] = at(init_cont, i);
  }

  return CSCM_MetaObject.gen(meta, Type, data);
}

static Object* top(Object* cont)
{
  return at(cont, size(cont) - 1);
}

static int size(Object* cont)
{
  return PULL(cont)->size;
}

static int max_size(Object* cont)
{
  return PULL(cont)->max_size;
}

static Object* at(Object* cont, int pos)
{
  assert(pos < size(cont));
  return PULL(cont)->stack[pos];
}

static void erase(Object* cont, int start_pos, int length)
{
  assert(start_pos < size(cont) && start_pos + length <= size(cont));

  for (int i = 0; i < length; i++)
    CSCM_MetaObject.unreferred(at(cont, start_pos + i));
  
  for (int i = start_pos + length; i < size(cont); i++)
    PULL(cont)->stack[i - length] = at(cont, i);

  PULL(cont)->size -= length;
}

static void pop(Object* cont)
{
  erase(cont, size(cont) - 1, 1);
}

static void push(Object* cont, Object* obj)
{
  assert(size(cont) < max_size(cont));
  CSCM_MetaObject.referred(PULL(cont)->stack[PULL(cont)->size++] = obj);
}

static void trans(Object* cont, Object* alt_cont, Object* obj)
{
  int s = size(cont);
  for (int i = 0; i < size(alt_cont); i++)
    push(cont, at(alt_cont, i));
  push(cont, obj);
  erase(cont, 0, s);
}

static void returnTopToForm(Object* cont)
{
  Object* form = at(cont, size(cont) - 2);
  assert(form->type == &CSCM_Form);
  CSCM_Form.back(form, top(cont));
  pop(cont);
}
  
static void popAndPush(Object*cont, Object* obj)
{
  push(cont, obj);
  erase(cont, size(cont) - 2, 1);
}

CSCM_Continuation_T CSCM_Continuation = {
  {release, apply, NULL, NULL},
  new, top, size, max_size, at,
  erase, pop, push, trans, returnTopToForm, popAndPush
};
