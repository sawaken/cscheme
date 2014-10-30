#include <stdlib.h>
#include <stdio.h>
#include "type/type.h"
#include "type/type_alias.h"
#include "eval.h"
#include "util.h"


static bool _if(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);
  
  if (Form.size(form) != 4) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "invalid arglen.")));
    return true;
  }

  if (Form.pos(form) != 2)
    return false;

  Object* p = Form.evaluatedElement(form, 1);
  Object* a = Form.rawElement(form, 2);
  Object* b = Form.rawElement(form, 3);
  Object* selected = Bool.select(p, a, b);

  CSCM_Eval.stackNextFrame(meta, cont, Form.env(form), selected);
  Continuation.erase(cont, Continuation.size(cont) - 2, 1);

  return true;
}

static bool call_cc(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);

  if (Form.size(form) != 2) {
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

  if (Form.size(form) < 3) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "invalid arglen.")));
    return true;
  }

  Object* param = Util.parseParam(meta, Form.rawElement(form, 1));

  if (param == NULL) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "invalid params.")));
  } else {
    Continuation.popAndPush(cont, Lambda.new(meta, Form.env(form), param,
					     Form.rawElements(form, 2), 
					     Form.size(form) - 2));
  }
  return true;
}

// temporary (because of unconsidering position)
static bool define(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);

  if (Form.size(form) != 3) {
    Continuation.push(cont, Exception.new(meta, String.new(meta, "invalid arglen.")));
    return true;
  }

  if (Form.pos(form) == 1) {
    if (!Util.isA(Form.next(form), &Symbol)) {
      Continuation.push(cont, Exception.new(meta, String.new(meta, "arg should be symbol.")));
      return true;
    } else {
      Form.back(form, Form.next(form));
      return true;
    }
  }
  
  if (Form.pos(form) == 3) {
    Env.bind(Form.env(form), Form.evaluatedElement(form, 1), Form.evaluatedElement(form, 2));
    Continuation.popAndPush(cont, Form.evaluatedElement(form, 1));
    return true;
  }

  return false;
}


#define BIND(meta, getSymbol, env, func, name) Env.bind((env), (getSymbol)((meta), name), \
							SpecialForm.new((meta), (name), (func)))

void CSCM_BindSF(Object* meta, Object* (*getSymbol)(Object*, const char*), Object* env)
{
  BIND(meta, getSymbol, env, _if,       "if");
  BIND(meta, getSymbol, env, call_cc,   "call/cc");
  BIND(meta, getSymbol, env, lambda,    "lambda");
  BIND(meta, getSymbol, env, define,    "define");
}

