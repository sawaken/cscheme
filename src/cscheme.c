#include <stdlib.h>

#include "type/type.h"
#include "type/type_alias.h"
#include "initial_bind.h"
#include "parse.h"
#include "eval.h"
#include "util.h"
#include "cscheme.h"

static CSCM_Interpreter* new(void)
{
  CSCM_Interpreter* inter = malloc(sizeof(CSCM_Interpreter));
  inter->meta = MetaObject.new(100);
  inter->top_env = Env.new(inter->meta, NULL);
  inter->evaluated = NULL;

  BindSF(inter->meta, Util.singletonSymbol, inter->top_env);
  BindPF(inter->meta, Util.singletonSymbol, inter->top_env);
  MetaObject.referred(inter->top_env);

  return inter;
}

static void ret(CSCM_Interpreter* inter, Object* evaluated)
{
  MetaObject.referred(evaluated);
  if (inter->evaluated != NULL)
    MetaObject.unreferred(inter->evaluated);
  inter->evaluated = evaluated;
}

static void eval(CSCM_Interpreter* inter, const char code[])
{
  Generator g = {inter->meta, Cell.new, Util.singletonSymbol, NULL};
  Object* exp = ParseExp(code, &g);
  Object* cont = Continuation.new(inter->meta, NULL);
  Object* form = Form.new(inter->meta, inter->top_env, exp, Util.length(exp), true);

  MetaObject.release(exp);
  Continuation.push(cont, form);
  
  Object* evaluated = CSCM_Eval.eval(inter->meta, cont);
  ret(inter, evaluated);
  MetaObject.release(cont);
}

static Object* getEvaluated(CSCM_Interpreter* inter)
{
  return inter->evaluated;
}

static void release(CSCM_Interpreter* inter)
{
  free(inter->meta);
  free(inter->top_env);
  if (inter->evaluated != NULL)
    free(inter->evaluated);
  free(inter);
}

static char* showLastEvaluated(CSCM_Interpreter * inter, char* buf)
{
  return Util.toStr(getEvaluated(inter), buf);
}


CSCM_T CSCM = {
  .New = new,
  .eval = eval,
  .ret = ret,
  .getEvaluated = getEvaluated,
  .release = release,
  .showLastEvaluated = showLastEvaluated,
};
  

