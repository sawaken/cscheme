#include "type/type.h"
#include "eval.h"

static bool _if(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);

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

  if (Form.pos(form) != 2)
    return false;

  Object* lambda = Form.evaluatedElemnt(form, 1);
  Object* cc = Continuation.new(meta, cont);

  Continuation.pop(cc);
  Continuation.replace(cont, Util.form(meta, Form.env(form), false, 2, lambda, cc));

  return true;
}

static bool lambda(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);
  Object* param = Util.parseParam(meta, Form.rawElement(form, 1));

  if (param == NULL) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "invalid params.")));
  } else {
    Continuation.replace(cont, Lambda.new(meta, param, Form.rawElements(form, 2), 
					  Form.size(form) - 2));
  }
}


void BindSF(Generator* g, Object* env)
{
  Env.bind(env, g->symbol(g->meta_obj, "if"), 
	   SpecialForm.new(meta, "if", _if, 2, 3));
  Env.bind(env, g->symbol(g->meta_obj, "call/cc"),
	   SpecialForm.new(meta, "call/cc", call_cc, 1, 1));
  Env.bind(env, g->symbol(g->meta_obj, "lambda"),
	   SpecialForm.new(meta, "lambda", lambda, 2, LEN_INF));
}
