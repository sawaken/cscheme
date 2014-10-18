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
  int (*Comp)(Object* a, Object* b);
} t_Util;
extern t_Util Util;

#endif
