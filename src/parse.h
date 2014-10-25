#include "type/type.h"

#ifndef PARSE_H
#define PARSE_H

typedef struct
{
  Object* meta_obj;
  Object* (*cons)(Object*, Object*, Object*);
  Object* (*symbol)(Object*, const char[]);
  Object* (*exception)(Object*, const char*);
} Generator;


#ifdef __cplusplus
extern "C" {
#endif

  Object* ParseExp(const char exp[], Generator* gen);

#ifdef __cplusplus
}
#endif

#endif
