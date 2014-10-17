#include <stdarg.h>
#include <stdlib.h>
#include "type/type.h"
#include "util.h"

static Object* i_list(Object* meta, int length, va_list ap)
{
  if (length > 0) {
    Object* a = va_arg(ap, Object*);
    return Cell.new(meta, a, i_list(meta, length - 1, ap));
  }
  else {
    return Cell.new(meta, NULL, NULL);
  }
}

static Object* list(Object* meta, int length, ...)
{
  va_list ap;
  va_start(ap, length);
  return i_list(meta, length, ap);
}

static Object* i_symList(Generator* g, int length, va_list ap)
{
  if (length > 0) {
    Object* a = g->symbol(g->meta_obj, va_arg(ap, char*));
    return g->cons(g->meta_obj, a, i_symList(g, length - 1, ap));
  }
  else {
    return g->cons(g->meta_obj, NULL, NULL);
  } 
}

static Object* symList(Generator* g, int length, ...)
{
  va_list ap;
  va_start(ap, length);
  return i_symList(g, length, ap);
}

static int length(Object* obj)
{
  if (IsA(obj, &Cell) && !Cell.empty(obj))
    return 1 + length(Cell.cdr(obj));
  else
    return 0;
}

static bool isList(Object* obj)
{
  if (!IsA(obj, &Cell))
    return false;
  if (Cell.empty(obj))
    return true;
  else
    return isList(Cell.cdr(obj));
}

static Object* form(Object* meta, Object* env, bool body, int length, ...)
{
  va_list ap;
  va_start(ap, length);

  Object* exp = i_list(meta, length, ap);
  Object* form = Form.new(meta, env, exp, length, body);

  MetaObject.release(exp);

  return form;
}

static Object* arrayToList(Object* meta, Object** objects, int length)
{
  if (length > 0)
    return Cell.new(meta, *objects, arrayToList(meta, objects + 1, length - 1));
  else
    return Cell.new(meta, NULL, NULL);
}

static void assign(Object* meta, Object* param, Object* env, Object** args, int argc)
{
  int paramc = Parameter.paramc(param);

  for (int i = 0; i < paramc; i++) {
    Env.bind(env, Parameter.at(param, i), args[i]);
  }

  if (Parameter.rest(param) != NULL) {
    Env.bind(env, Parameter.rest(param), 
	     Util.arrayToList(meta, args + paramc, argc - paramc));
  }
}

t_Util Util = {
  list, symList, length, isList, form, arrayToList, assign
};
