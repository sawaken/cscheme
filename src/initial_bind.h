#include "type/type.h"

#ifndef INITIAL_BIND_H
#define INITIAL_BIND_H

#ifdef __cplusplus
extern "C" {
#endif

  void BindSF(Object* meta, Object* (*getSymbol)(Object*, const char*), Object* env);
  void BindPF(Object* meta, Object* (*getSymbol)(Object*, const char*), Object* env);

#ifdef __cplusplus
}
#endif

#endif


