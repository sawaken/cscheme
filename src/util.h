#include <stdarg.h>
#include "type/type.h"

#ifndef UTIL_H
#define UTIL_H


typedef struct
{
  Object* (*list)(Object* meta, int length, ...);
  Object* (*symList)(Generator* g, int length, ...);
  int (*length)(Object* obj);
  bool (*isList)(Object* obj);
  Object* (*form)(Object* meta, Object* env, bool body, int length, ...);
  Object* (*arrayToList)(Object* meta, Object** objects, int length);
  void (*assign)(Object* meta, Object* param, Object* env, Object** args, int argc);
  int (*comp)(Object* a, Object* b);
  Object* (*singletonSymbol)(Object* meta, const char* name);
  bool (*include)(Object* list, Object* obj, int (*comp)(Object*, Object*));
  bool (*listDup)(Object* list);
  void (*listToArray)(Object* list, Object* array[]);
  Object* (*ith)(Object* list, int i);
  Object* (*parseParam)(Object* meta, Object* param_list, const char* dot);
  bool (*isAll)(Object** args, int argc, void* type);
  char* (*toStr)(Object* obj, char buf[]);
} t_Util;
extern t_Util Util;

#endif
