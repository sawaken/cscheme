#include "type/type.h"

#ifndef EVAL_H
#define EVAL_H

void TailCallOptimize(Object* meta, Object* cont, Object* tail);
void StackNextFrame(Object* meta, Object* cont, Object* next);
void ApplyContinuation(Object* meta, Object* cont, Object* top);
Object* MakeForm(Object* meta, Object* lambda, Object** args, int argc);
void ApplyLambda(Object* meta, Object* cont, Object* lambda,
		 Object** args, int argc);
void Apply(Object* meta, Object* cont, Object* form);
bool StackOperation(Object* meta, Object* cont, Object* top);
void Raise(Object* meta, Object* cont);
Object* Eval(Object* meta, Object* cont);

#endif
