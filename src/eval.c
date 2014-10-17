#include <stdlib.h>
#include "type/type.h"
#include "util.h"
#include "eval.h"

void TailCallOptimize(Object* meta, Object* cont, Object* env, Object* tail)
{
  if (IsA(tail, &Cell)) {
    Continuation.popAndPush(cont, Form.new(meta, env, tail, Util.length(tail), false));
  } else {
    Continuation.popAndPush(cont, tail);
  }
}

void StackNextFrame(Object* meta, Object* cont, Object* env, Object* next)
{
  if (IsA(next, &Cell)) {
    Continuation.push(cont, Form.new(meta, env, next, Util.length(next), false));
  }
  else if (IsA(next, &Symbol)) {
    Object* solved = Env.find(env, next, Util.EQ);
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
  Object* exp = Util.arrayToList(meta, Lambda.exps(lambda), Lambda.expc(lambda));
  Object* env = Env.new(meta, Lambda.env(lambda));
  Object* form = Form.new(meta, env, exp, Lambda.expc(lambda), true);
  
  Util.assign(meta, Lambda.param(lambda), env, args, argc);
  MetaObject.release(exp);

  return form;
}


void ApplyLambda(Object* meta, Object* cont, Object* lambda,
		 Object** args, int argc)
{
  if (Parameter.validArgLength(Lambda.param(lambda), argc)) {
    Continuation.popAndPush(cont, MakeForm(meta, lambda, args, argc));
  } else {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "lambda arg error.")));
  }
}

void Apply(Object* meta, Object* cont, Object* form)
{
  Object* command = Form.evaluatedElement(form, 0);

  if (IsA(command, &Lambda)) {
    ApplyLambda(meta, cont, command,
		Form.evaluatedElements(form, 1),
		Form.pos(form) - 1);
    return;
  }
    
  if (IsA(command, &PrimFunc)) {
    Continuation.popAndPush(cont, PrimFunc.apply(command,
					      meta,
					      Form.evaluatedElements(form, 1),
					      Form.pos(form) - 1));
    return;
  }

  if (IsA(command, &Continuation)) {
    ApplyContinuation(meta, cont, form);
    return;
  }

  Continuation.push(cont, Exception.new(meta, String.new(meta, "apply error.")));
}

bool StackOperation(Object* meta, Object* cont, Object* top)
{
  if (IsA(top, &Exception)) {
    Raise(meta, cont);
    return true;
  }

  if (!IsA(top, &Form)) {
    Continuation.returnTopToForm(cont);
    return true;
  }

  if (Form.isBody(top) && Form.restNum(top) == 1) {
    TailCallOptimize(meta, cont, Form.env(top), Form.next(top));
    return true;
  }
    
  if (Form.pos(top) > 0 && IsA(Form.evaluatedElement(top, 0), &SpecialForm)) {
    if (SpecialForm.doAction(Form.evaluatedElement(top, 0), meta, cont)) {
      return true;
    }
  }
  
  return false;
}

void Raise(Object* meta, Object* cont)
{
  Continuation.erase(cont, 0, Continuation.size(cont) - 1);
}

Object* Eval(Object* meta, Object* cont)
{
  while (true) {
    Object* top = Continuation.top(cont);

    if (!IsA(top, &Form) && Continuation.size(cont) == 1) {
      return top;
    }

    if (StackOperation(meta, cont, top))
      continue;

    if (Form.restNum(top) > 0) {
      StackNextFrame(meta, cont, Form.env(top), Form.next(top));
    } else {
      Apply(meta, cont, top);
    }
  }
}
