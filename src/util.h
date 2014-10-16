#include <stdarg.h>
#include "type/type.h"

#ifndef UTIL_H
#define UTIL_H


typedef struct
{
  Object* (*list)(Generator* g, int length, ...);
  Object* (*symList)(Generator* g, int length, ...);
} t_Util;
extern t_Util Util;

#endif
