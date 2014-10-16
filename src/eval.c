#include "type/type.h"
#include "eval_util.h"

Object* eval(Object* meta, Object* cont)
{
  while (true) {
    Object *command, *top = Continuation.top(cont);

    if (!IsA(top, &Form) && Continuation.size(cont) == 1) {
      return Continuation.top(cont);
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
      SpecialForm.doAction(Form.evaluatedElement(top, 0), meta, cont);
      continue;
    }

    if (Form.rest(top) > 0) {
      NextStackFrame(meta, cont, Form.next(top));
      continue;
    }

    command = Form.evaluatedElement(top, 0);

    if (IsA(command, &Lambda)) {
      ApplyLambda(meta, cont, command,
		  Form.evaluatedElements(top, 1),
		  Form.pos(top) - 1);
      continue;
    }
    
    if (IsA(command, &PrimFunc)) {
      Continuation.replace(cont, PrimFunc.apply(command,
						Form.evaluatedElements(top, 1),
						Form.pos(top) - 1));
      continue;
    }

    if (IsA(command, &Continuation)) {
      ApplyContinuation(meta, cont, top);
      continue;
    }

    Continuation.push(cont, Exception.new(meta, String.new(meta, "apply error.")));
  }
}
