#include "type/type.h"

#ifndef EVAL_H
#define EVAL_H

typedef struct
{
  void (*tailCallOptimize)(Object* meta, Object* cont, Object* env, Object* tail);
  void (*stackNextFrame)(Object* meta, Object* cont, Object* env, Object* next);
  void (*applyContinuation)(Object* meta, Object* cont, Object* alt_cont, Object* top);
  Object* (*makeForm)(Object* meta, Object* lambda, Object** args, int argc);
  void (*applyLambda)(Object* meta, Object* cont, Object* lambda,
		      Object** args, int argc);
  void (*apply)(Object* meta, Object* cont, Object* form);
  bool (*stackOperation)(Object* meta, Object* cont, Object* top);
  void (*raise)(Object* meta, Object* cont);
  Object* (*eval)(Object* meta, Object* cont);
} CSCM_Eval_T;

extern CSCM_Eval_T CSCM_Eval;

#endif
