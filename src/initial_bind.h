#include "type/type.h"

#ifndef INITIAL_BIND_H
#define INITIAL_BIND_H

#ifdef __cplusplus
extern "C" {
#endif

  void BindSF(Generator* g, Object* env);
  void BindPF(Generator* g, Object* env);

#ifdef __cplusplus
}
#endif

#endif


