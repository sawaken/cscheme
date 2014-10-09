#include "type/type.h"

void eval(Object* meta, Object* cont)
{
  while (true) {
    Object* form = Continuation.top(cont);

    if (!IsA(Continuation.top(cont), &Form)) {
      if (Continuation.size(cont) == 1) return Continuation.top(cont);
      Continuation.ret(cont);
      continue;
    }

    if (Form.isBody(form) && Form.restNum(form) == 1) {
      Object* tail = Form.next(form);
      if (IsA(tail, &Cell)) {
	Continuation.replace(cont, Form.new(meta, tail));
      } else {
	Continuation.replace(cont, tail);
      }
      continue;
    }
    
    if (Form.commandEvaluated(form) && IsA(Form.command(form), &SpecialForm)) {
      SpecialForm.doAction(meta, Form.command(form), cont);
      continue;
    }

    if (Form.hasUnevaluated(form)) {
      Object* next = Form.next(form);

      if (IsA(next, &Cell)) {
	Continuation.push(cont, Form.new(meta, Form.env(form), next));
      }
      else if (IsA(next, &Symbol)) {
	Form.back(form, Env.resolve(Form.env(form), next));
      }
      else {
	Form.back(form, next);
      }
    }
    else {    // this level's are all evaluated. so apply.
      Object* command = Form.command(form);

      if (IsA(command, &Lambda)) {
	// arg_len check
	Continuation.replace(cont, Lambda.makeForm(meta, command, form));
      }
      else if (IsA(command, &PrimFunc)) {
	Object* obj = PrimFunc.apply(command, form);
	if (IsA(obj, &Exception)) {
	  Continuation.replace(cont, obj);
	  continue;
	}
	Continuation.replace(cont, obj);
      }
      else if (IsA(command, &Continuation)) {
	// arg_len check
	Continuation.call(cont, command, Form.evaluatedArg(form, 1));
      }
      else {
	assert(false);
      }
    }

  }
}
