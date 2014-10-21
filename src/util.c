#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

static int comp(Object* a, Object* b)
{
  if (a == b) return 0;
  if (a < b) return 1;
  else return -1;
}

static Object* singletonSymbol(Object* meta, const char* name)
{
  Object* existing = MetaObject.findSymbol(meta, name, strcmp);

  if (existing != NULL)
    return existing;
  else
    return Symbol.new(meta, name);
}

static int i_dotPos(int i, Object* list, const char* dot)
{
  if (Cell.empty(list))
    return -1;
  if (strcmp(Symbol.to_s(Cell.car(list)), dot) == 0)
    return i;
  else
    return i_dotPos(i + 1, Cell.cdr(list), dot);
}

static bool include(Object* list, Object* obj, int (*comp)(Object*, Object*))
{
  return !Cell.empty(list)
    && (comp(Cell.car(list), obj) == 0 || include(Cell.cdr(list), obj, comp));
}

static bool listDup(Object* list)
{
  return !Cell.empty(list)
    && (include(Cell.cdr(list), Cell.car(list), comp) || listDup(Cell.cdr(list)));
}

static void listToArray(Object* list, Object* array[])
{
  if (!Cell.empty(list)){
    *array = Cell.car(list);
    listToArray(Cell.cdr(list), array + 1);
  }
}

static Object* ith(Object* list, int i)
{
  if (Cell.empty(list))
    return NULL;
  if (i == 0)
    return Cell.car(list);
  else
    return ith(Cell.cdr(list), i - 1);
}

static Object* parseParam(Object* meta, Object* param_list, const char* dot)
{
  int len = length(param_list);
  if (listDup(param_list) || len > 100)
    return NULL;

  int dot_pos = i_dotPos(0, param_list, dot);
  if (dot_pos != -1 && dot_pos != length(param_list) - 2)
    return NULL;

  Object* rest;
  if (dot_pos == -1) {
    rest = NULL;
  } else {
    rest = ith(param_list, dot_pos + 1);
    len = dot_pos;
  }

  Object* param_array[100];
  listToArray(param_list, param_array);

  return Parameter.new(meta, param_array, len, rest);
}

static bool isAll(Object** args, int argc, void* type)
{
  for (int i = 0; i < argc; i++)
    if (!IsA(args[i], type))
      return false;
  return true;
}


// temporary implimentation
static char* toStr(Object* obj, char buf[])
{
  if (IsA(obj, &Integer)) {
    snprintf(buf, 100, "%d", Integer.to_i(obj));
  } else if (IsA(obj, &Symbol)) {
    snprintf(buf, 100, "<#symbol: %s>", Symbol.to_s(obj));
  } else if (IsA(obj, &String)) {
    snprintf(buf, 100, "<#string: %s>", String.to_s(obj));
  } else if (IsA(obj, &Exception)) {
    snprintf(buf, 100, "<#exception: %s>", String.to_s(Exception.take(obj)));
  } else {
    snprintf(buf, 100, "<#object>");
  }

  return buf;
}


t_Util Util = {
  list, symList, length, isList, form, arrayToList, assign, comp, singletonSymbol,
  include, listDup, listToArray, ith, parseParam, isAll, toStr
};
