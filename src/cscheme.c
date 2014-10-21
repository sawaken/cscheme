#include <stdlib.h>

#include "type/type.h"
#include "initial_bind.h"
#include "parse.h"
#include "eval.h"
#include "util.h"
#include "cscheme.h"

static Interpreter* new(void)
{
  Interpreter* inter = malloc(sizeof(Interpreter));
  inter->meta = MetaObject.new(100);
  inter->top_env = Env.new(inter->meta, NULL);
  inter->evaluated = NULL;

  Generator g = {inter->meta, Cell.new, Util.singletonSymbol, NULL};
  BindSF(&g, inter->top_env);
  BindPF(&g, inter->top_env);
  MetaObject.referred(inter->top_env);

  return inter;
}

static void ret(Interpreter* inter, Object* evaluated)
{
  MetaObject.referred(evaluated);
  if (inter->evaluated != NULL)
    MetaObject.unreferred(inter->evaluated);
  inter->evaluated = evaluated;
}

static void eval(Interpreter* inter, const char code[])
{
  Generator g = {inter->meta, Cell.new, Util.singletonSymbol, NULL};
  Object* exp = ParseExp(code, &g);
  Object* cont = Continuation.new(inter->meta, NULL);
  Object* form = Form.new(inter->meta, inter->top_env, exp, Util.length(exp), true);

  MetaObject.release(exp);
  Continuation.push(cont, form);
  
  Object* evaluated = Eval(inter->meta, cont);
  ret(inter, evaluated);
  MetaObject.release(cont);
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

static char* showLastEvaluated(Interpreter * inter, char* buf)
{
  return Util.toStr(getEvaluated(inter), buf);
}


t_Cscheme Cscheme = {
  new, eval, ret, getEvaluated, release, showLastEvaluated
};
  

