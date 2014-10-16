#include "type/type.h"

#ifndef EVAL_UTIL_H
#define EVAL_UTIL_H

void TailCallOptimize(Object* meta, Object* cont, Object* tail);
void NextStackFrame(Object* meta, Object* cont, Object* next);
void ApplyContinuation(Object* meta, Object* cont, Object* top);
Object* MakeForm(Object* meta, Object* lambda, Object** args, int argc);
void ApplyLambda(Object* meta, Object* cont, Object* lambda,
		 Object** args, int argc);

#endif
