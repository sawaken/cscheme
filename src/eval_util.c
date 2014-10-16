#include "type/type.h"
#include "eval_util.h"

void TailCallOptimize(Object* meta, Object* cont, Object* tail)
{
  Object* top = continouation.top(cont);

  if (IsA(tail, &Cell)) {
    Continuation.replace(cont, Form.new(meta, Form.env(top), tail, LS.length(tail)));
  } else {
    Continuation.replace(cont, tail);
  }
}

void NextStackFrame(Object* meta, Object* cont, Object* next)
{
  Object* top = continouation.top(cont);

  if (IsA(next, &Cell)) {
    Continuation.push(cont, Form.new(meta, Form.env(top), next, LS.length(next)));
  }
  else if (IsA(next, &Symbol)) {
    Object* solved = Env.find(Form.env(top), next);
    if (solved != NULL) {
      Continuation.push(cont, solved);
    } else {
      Continuation.push(cont, Exception.new(meta, String.new(meta, "unsolvable.")));
    }
  }
  else {
    Continuation.push(cont, next);
  }
}

void ApplyContinuation(Object* meta, Object* cont, Object* top)
{
  if (Form.pos(top) != 2) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "arg error.")));
  } else {
    Continuation.trans(cont,
		       Form.evaluatedElement(top, 0),
		       Form.evaluatedElement(top, 1));
  }
}

Object* MakeForm(Object* meta, Object* lambda, Object** args, int argc)
{


}

void ApplyLambda(Object* meta, Object* cont, Object* lambda,
		 Object** args, int argc)
{
  if (Parameter.validArgc(Lambda.parameter(lambda), argc)) {
    Continuation.replace(cont, MakeForm(meta, lambda, args, argc));
  } else {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "lambda arg error.")));
  }
}
