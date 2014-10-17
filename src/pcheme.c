#include "type/type.h"
#include "pcheme.h"

static Interpreter* new(void)
{
  Interpreter* inter = malloc(sizeof(Interpreter));
  inter->meta = MetaObject.new(100);
  inter->top_env = Env.new(inter->meta, NULL);
  inter->evaluated = NULL;
  BindSF(inter->meta, inter->top_env);
  BindPF(inter->meta, inter->top_env);
}

static void eval(Interpreter* inter, const char code[])
{
  Generator g = {inter->meta, Cell.new, Symbol.new, Exception.new};
  Object* exp = Parse(code, &g);
  Object* cont = Continuation.new(inter->meta);
  Object* form = Form.new(inter->meta, inter->env, exp, Util.length(exp), false);

  MetaObject.release(exp);
  Continuation.push(cont, form);
  
  Object* evaluated = Eval(inter->meta, cont);
  Pcheme.ret(inter, evaluated);
  MetaObject.release(cont);
}

static void ret(Interpreter* inter, Object* evaluated)
{
  MetaObject.referred(evaluated);
  if (inter->evaluated != NULL)
    MetaObject.unreferred(inter->evaluated);
  inter->evaluated = evaluated;
}

static Object* getEvaluated(Interpreter* inter)
{
  return inter->evaluated;
}

static void release(Interpreter* inter)
{
  free(inter->meta);
  free(inter->top_env);
  if (inter->evaluated != NULL)
    free(inter->evaluated);
  free(inter);
}

t_Pcheme Pcheme = {
  new, eval, ret, getEvaluated, release
};
  

