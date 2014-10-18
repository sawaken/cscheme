#include <stdlib.h>
#include "type/type.h"
#include "eval.h"
#include "util.h"

static int arglen(Object* form)
{
  return Form.pos(form) + Form.restNum(form) - 1;
}

static bool _if(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);
  
  if (arglen(form) < 1 || 2 < arglen(form)) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "invalid arglen.")));
    return true;
  }

  if (Form.pos(form) != 2)
    return false;

  Object* p = Form.evaluatedElement(form, 1);
  Object* a = Form.rawElement(form, 2);
  Object* b = Form.rawElement(form, 3);
  Object* selected = Bool.select(p, a, b);

  StackNextFrame(meta, cont, Form.env(form), selected);
  Continuation.erase(cont, Continuation.size(cont) - 2, 1);

  return true;
}

static bool call_cc(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);

  if (arglen(form) != 1) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "invalid arglen.")));
    return true;
  }

  if (Form.pos(form) != 2)
    return false;

  Object* lambda = Form.evaluatedElement(form, 1);
  Object* cc = Continuation.new(meta, cont);

  Continuation.pop(cc);
  Continuation.popAndPush(cont, Util.form(meta, Form.env(form), false, 2, lambda, cc));

  return true;
}

static bool lambda(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);

  if (arglen(form) < 2) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "invalid arglen.")));
    return true;
  }

  Object* param = Util.parseParam(meta, Form.rawElement(form, 1), ".");

  if (param == NULL) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "invalid params.")));
  } else {
    Continuation.popAndPush(cont, Lambda.new(meta, Form.env(form), param,
					     Form.rawElements(form, 2), 
					     arglen(form) - 1));
  }
}


void BindSF(Generator* g, Object* env)
{
  Env.bind(env, g->symbol(g->meta_obj, "if"), 
	   SpecialForm.new(g->meta_obj, "if", _if));
  Env.bind(env, g->symbol(g->meta_obj, "call/cc"),
	   SpecialForm.new(g->meta_obj, "call/cc", call_cc));
  Env.bind(env, g->symbol(g->meta_obj, "lambda"),
	   SpecialForm.new(g->meta_obj, "lambda", lambda));
}
