#include "type/type.h"
#include "eval.h"

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
    Continuation.replace(cont, PrimFunc.apply(command,
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

Object* Eval(Object* meta, Object* cont)
{
  while (true) {
    Objec* top = Continuation.top(cont);

    if (!IsA(top, &Form) && Continuation.size(cont) == 1) {
      return top;
    }

    if (IsA(top, &Exception)) {
      
    }

    if (!IsA(top, &Form)) {
      Continuation.returnTopToForm(cont);
      continue;
    }

    if (Form.isBody(top) && Form.restNum(top) == 1) {
      TailCallOptimize(meta, cont, Form.next(top));
      continue;
    }
    
    if (Form.pos(top) > 0 && IsA(Form.evaluatedElement(top, 0), &SpecialForm)) {
      if (SpecialForm.doAction(Form.evaluatedElement(top, 0), meta, cont)) {
	continue;
      }
    }

    if (Form.rest(top) > 0) {
      NextStackFrame(meta, cont, Form.next(top));
      continue;
    }

    Apply(meta, cont, top);
  }
}
