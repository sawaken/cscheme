#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type.h"

static void* Type = &Continuation;

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
  free(pull(obj)->stack);
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
    data->size = Continuation.size(init_cont);
    data->max_size = Continuation.max_size(init_cont);
    data->stack = malloc(data->max_size * sizeof(Object*));
    for (int i = 0; i < data->size; i++)
      data->stack[i] = Continuation.at(init_cont, i);
  }

  return MetaObject.gen(meta, Type, data);
}

static Object* top(Object* cont)
{
  return at(cont, size(cont) - 1);
}

static int size(Object* cont)
{
  return pull(cont)->size;
}

static int max_size(Object* cont)
{
  return pull(cont)->max_size;
}

static Object* at(Object* cont, int pos)
{
  assert(pos < size(cont));
  return pull(cont)->stack[pos];
}

static void erase(Object* cont, int start_pos, int length)
{
  assert(start_pos < size(cont) && start_pos + length <= size(cont));

  for (int i = 0; i < length; i++)
    MetaObject.unreferred(at(cont, start_pos + i));
  
  for (int i = start_pos + length; i < size(cont); i++)
    pull(cont)->stack[i - length] = at(cont, i);

  pull(cont)->size -= length;
}

static void pop(Object* cont)
{
  erase(cont, size(cont) - 1, 1);
}

static void push(Object* cont, Object* obj)
{
  assert(size(cont) < max_size(cont));
  MetaObject.referred(pull(cont)->stack[pull(cont)->size++] = obj);
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
  assert(IsA(at(cont, size(cont) - 2), &Form));
  Form.back(at(cont, size(cont) - 2), top(cont));
  pop(cont);
}
  
static void popAndPush(Object*cont, Object* obj)
{
  push(cont, obj);
  erase(cont, size(cont) - 2, 1);
}

t_Continuation Continuation = {
  {release, apply, NULL, NULL},
  new, top, size, max_size, at,
  erase, pop, push, trans, returnTopToForm, popAndPush
};
