#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "type/type.h"
#include "type/type_alias.h"
#include "util.h"

static bool isA(Object* obj, void* type)
{
  return obj->type == type;
}


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

static Object* i_symList(Object* meta, Object* (*getSymbol)(Object*, const char*), int length, va_list ap)
{
  if (length > 0) {
    Object* a = getSymbol(meta, va_arg(ap, char*));
    return Cell.new(meta, a, i_symList(meta, getSymbol, length - 1, ap));
  }
  else {
    return Cell.new(meta, NULL, NULL);
  } 
}

static Object* symList(Object* meta, Object* (*getSymbol)(Object*, const char*), int length, ...)
{
  va_list ap;
  va_start(ap, length);
  return i_symList(meta, getSymbol, length, ap);
}

static int length(Object* obj)
{
  if (!isA(obj, &Cell) || Cell.empty(obj))
    return 0;
  else
    return 1 + length(Cell.cdr(obj));
}

static bool isList(Object* obj)
{
  if (!isA(obj, &Cell))
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

// TODO: (a b . c) should be processed by Parser as NotAuthenticList. 
// (That means "." is literal for Parser.)
// So, parseParam should not (cannnot) handle symbol of ".".
// All parseParam should do is judging which Authetic or NotAuthetic param_list is.
static Object* parseParam(Object* meta, Object* param_list)
{
  char dot[] = ".";

  if (isA(param_list, &Symbol)) {
    return Parameter.new(meta, NULL, 0, param_list);
  }

  if (!isA(param_list, &Cell)) {
    return NULL;
  }

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
    if (!isA(args[i], type))
      return false;
  return true;
}

static bool isNonAuthenticList(Object* obj)
{
  return isA(obj, &Cell) && !isList(obj);
}

static Object* drop(Object* list, int n)
{
  if (n == 0)
    return list;
  else
    return drop(Cell.cdr(list), n - 1);
}
  

// temporary implimentation
static char* toStr(Object* obj, char buf[])
{
  if (isA(obj, &Integer)) {
    snprintf(buf, 100, "%d", Integer.to_i(obj));
  } else if (isA(obj, &Symbol)) {
    snprintf(buf, 100, "<#symbol: %s>", Symbol.to_s(obj));
  } else if (isA(obj, &String)) {
    snprintf(buf, 100, "<#string: %s>", String.to_s(obj));
  } else if (isA(obj, &Exception)) {
    snprintf(buf, 100, "<#exception: %s>", String.to_s(Exception.take(obj)));
  } else {
    snprintf(buf, 100, "<#object>");
  }

  return buf;
}


t_Util Util = {
  .isA = isA,
  .list = list,
  .symList = symList,
  .length = length,
  .isList = isList,
  .form = form,
  .arrayToList = arrayToList,
  .assign = assign,
  .comp = comp,
  .singletonSymbol = singletonSymbol,
  .include = include,
  .listDup = listDup,
  .listToArray = listToArray,
  .ith = ith,
  .parseParam = parseParam,
  .isAll = isAll,
  .toStr = toStr,
  .isNonAuthenticList = isNonAuthenticList,
  .drop = drop,
};
