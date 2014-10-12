#include <stdarg.h>
#include "type/type.h"

#ifndef LIST_UTIL_H
#define LIST_UTIL_H


typedef struct
{
  Object* (*New)(Generator* g, int length, ...);
  Object* (*symList)(Generator* g, int length, ...);
} ListUtil;
extern ListUtil LS;

#endif
