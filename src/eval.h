#include "type/type.h"

#ifndef EVAL_H
#define EVAL_H

#ifdef __cplusplus
extern "C" {
#endif

  void TailCallOptimize(Object* meta, Object* cont, Object* env, Object* tail);
  void StackNextFrame(Object* meta, Object* cont, Object* env, Object* next);
  void ApplyContinuation(Object* meta, Object* cont, Object* alt_cont, Object* top);
  Object* MakeForm(Object* meta, Object* lambda, Object** args, int argc);
  void ApplyLambda(Object* meta, Object* cont, Object* lambda,
		   Object** args, int argc);
  void Apply(Object* meta, Object* cont, Object* form);
  bool StackOperation(Object* meta, Object* cont, Object* top);
  void Raise(Object* meta, Object* cont);
  Object* Eval(Object* meta, Object* cont);

#ifdef __cplusplus
}
#endif

#endif
