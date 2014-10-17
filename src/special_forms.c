#include "type/type.h"

static bool _if(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);

  if (Form.pos(form) != 2)
    return false;

    Object* p = Form.evaluatedElement(form, 1);
    Object* a = Form.rawElement(form, 2);
    Object* b = Form.rawElement(form, 3);
    Object* selected = Bool.which(p, a, b);

    if (IsA(selected, &Cell)) {
      Continuation.replace(cont, Form.new(meta, selected));
    } else {
      Continuation.replace(cont, selected);
    }
  }
}

// Arg1 should be [Lambda]
static bool call_cc(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);

  if (Form.pos(form) != 2)
    return false;

  Object* lambda = Form.evaluatedArg(form, 1); //type check
  Object* cc = Continuation.dup(cont);
  Continuation.pop(cc);
  Continuation.replace(cont, Lambda.makeForm(meta, lambda, &cc, 1));
  return true;
}

// Arg1 should be [Symbol]
static bool lambda(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);
  Object* lambda = Lambda.new(meta,
			      Form.rawArg(form, 1), //type check
			      Form.elements(form, 2),
			      Form.restNum(form) - 1)
  Continuation.replace(cont, lambda);
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
