#include <stdio.h>

#include "type/type.h"
#include "util.h"

#define def(name) static Object* (name)(Object* meta, Object** args, int argc)
#define EX(message) Exception.new(meta, String.new(meta, (message)))

def(sum_int)
{
  if (!Util.isAll(args, argc, &Integer)) {
    return EX("all args should be integer.");
  } else {
    return Integer.sum(meta, args, argc);
  }
}

def(display)
{
  for (int i = 0; i < argc; i++) {
    char buf[100];
    printf("%s\n", Util.toStr(args[i], buf));
  } 
  return args[argc - 1];
}

void BindPF(Generator* g, Object* env)
{
  Env.bind(env, g->symbol(g->meta_obj, "+"),
	   PrimFunc.new(g->meta_obj, "+", sum_int));
  Env.bind(env, g->symbol(g->meta_obj, "display"),
	   PrimFunc.new(g->meta_obj, "display", display));
}
