#include <stdlib.h>
#include <assert.h>
#include "type/type.h"
#include "type/type_alias.h"
#include "util.h"
#include "eval.h"

#include <stdio.h>

static void raise(Object* meta, Object* cont);
static void stackNextFrame(Object* meta, Object* cont, Object* env, Object* next);


static void tailCallOptimize(Object* meta, Object* cont, Object* env, Object* tail)
{
  stackNextFrame(meta, cont, env, tail);
  Continuation.erase(cont, Continuation.size(cont) - 2, 1);
}

static void stackNextFrame(Object* meta, Object* cont, Object* env, Object* next)
{
  if (Util.isA(next, &Cell)) {
    Continuation.push(cont, Form.new(meta, env, next, Util.length(next), false));
  }
  else if (Util.isA(next, &Symbol)) {
    Object* solved = Env.find(env, next, Util.comp);
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

static void applyContinuation(Object* meta, Object* cont, Object* alt_cont, Object* form)
{
  assert(Form.restNum(form) == 0);

  if (Form.pos(form) != 2) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "arg error.")));
  } else {
    Continuation.trans(cont, alt_cont, Form.evaluatedElement(form, 1));
  }
}

static Object* makeForm(Object* meta, Object* lambda, Object** args, int argc)
{
  Object* exp = Util.arrayToList(meta, Lambda.exps(lambda), Lambda.expc(lambda));
  Object* env = Env.new(meta, Lambda.env(lambda));
  Object* form = Form.new(meta, env, exp, Lambda.expc(lambda), true);
  
  Util.assign(meta, Lambda.param(lambda), env, args, argc);
  MetaObject.release(exp);

  return form;
}


static void applyLambda(Object* meta, Object* cont, Object* lambda,
		 Object** args, int argc)
{
  if (Parameter.validArgLength(Lambda.param(lambda), argc)) {
    Continuation.popAndPush(cont, makeForm(meta, lambda, args, argc));
  } else {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "lambda arg error.")));
  }
}

static void apply(Object* meta, Object* cont, Object* form)
{
  assert(Form.restNum(form) == 0);

  Object* command = Form.evaluatedElement(form, 0);

  if (Util.isA(command, &Lambda)) {
    applyLambda(meta, cont, command,
		Form.evaluatedElements(form, 1),
		Form.pos(form) - 1);
    return;
  }

  if (Util.isA(command, &PrimFunc)) {
    Continuation.popAndPush(cont, PrimFunc.apply(command,
						 meta,
						 Form.evaluatedElements(form, 1),
						 Form.pos(form) - 1));
    return;
  }

  if (Util.isA(command, &Continuation)) {
    applyContinuation(meta, cont, command, form);
    return;
  }

  Continuation.push(cont, Exception.new(meta, String.new(meta, "apply error.")));
}

static bool stackOperation(Object* meta, Object* cont, Object* top)
{
  if (Util.isA(top, &Exception)) {
    raise(meta, cont);
    return true;
  }

  if (!Util.isA(top, &Form)) {
    Continuation.returnTopToForm(cont);
    return true;
  }

  if (Form.isBody(top) && Form.restNum(top) == 1) {
    tailCallOptimize(meta, cont, Form.env(top), Form.next(top));
    return true;
  }
    
  if (Form.pos(top) > 0 && Util.isA(Form.evaluatedElement(top, 0), &SpecialForm)) {
    if (SpecialForm.doAction(Form.evaluatedElement(top, 0), meta, cont)) {
      return true;
    }
  }
  
  return false;
}

static void raise(Object* meta, Object* cont)
{
  Continuation.erase(cont, 0, Continuation.size(cont) - 1);
}

static Object* eval(Object* meta, Object* cont)
{
  while (true) {
    Object* top = Continuation.top(cont);

    if (!Util.isA(top, &Form) && Continuation.size(cont) == 1) {
      return top;
    }

    if (stackOperation(meta, cont, top))
      continue;

    if (Form.restNum(top) > 0) {
      stackNextFrame(meta, cont, Form.env(top), Form.next(top));
    } else {
      apply(meta, cont, top);
    }
  }
}

CSCM_Eval_T CSCM_Eval = {
  .tailCallOptimize = tailCallOptimize,
  .stackNextFrame = stackNextFrame,
  .applyContinuation = applyContinuation,
  .makeForm = makeForm,
  .applyLambda = applyLambda,
  .apply = apply,
  .stackOperation = stackOperation,
  .raise = raise,
  .eval = eval,
};
