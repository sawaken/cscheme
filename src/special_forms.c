#include "type/type.h"

Object* Body_f, If_f, CallCC_f, Lambda_f;


static void body(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);
  if (Form.restNum(form) == 1) {

    Object* tail = Form.next(form);

    if (IsA(tail, &Cell)) {
      Continuation.replace(cont, Form.new(meta, tail));
    } else {
      Continuation.replace(cont, tail);
    }
  }
}

static void _if(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);
  if (Form.pos(form) == 2) {
    Object* p = Form.evaluatedArg(form, 1);
    Object* a = Form.rawArg(form, 2);
    Object* b = Form.rawArg(form, 3);
    Object* exp = Bool.which(p, a, b);

    if (IsA(exp, &Cell)) {
      Continuation.replace(cont, Form.new(meta, exp));
    } else {
      Continuation.replace(cont, exp);
    }
  }
}

// Arg1 should be [Lambda]
static void call_cc(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);
  if (Form.pos(form) == 2) {
    Object* lambda = Form.evaluatedArg(form, 1); //type check
    Object* cc = Continuation.dup(cont);
    Continuation.pop(cc);
    Continuation.replace(cont, Lambda.makeForm(meta, lambda, &cc, 1));
  }
}

// Arg1 should be [Symbol]
static void lambda(Object* meta, Object* cont)
{
  Object* form = Continuation.top(cont);
  Object* lambda = Lambda.new(meta,
			      Form.rawArg(form, 1), //type check
			      Form.elements(form, 2),
			      Form.restNum(form) - 1)
  Continuation.replace(cont, lambda);
}

void InitSpecialForms(Object* meta)
{
  Body_f   = SpecialForm.new(meta, "body",    body,     1, LEN_INF);
  If_f     = SpecialForm.new(meta, "if",      _if,      2, 3);
  CallCC_f = SpecialForm.new(meta, "call/cc", call_cc,  1, 1);
  Lambda_f = SpecialForm.new(meta, "lambda",  lambda,   2, LEN_INF);
}

