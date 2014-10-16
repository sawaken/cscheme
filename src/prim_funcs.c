#include "type/type.h"

#define def(name) (static Object* (name)(Object* meta, Object** args, int argc))
#define EX(message) Exception.new(meta, String.new(meta, (message)))

def(sum_int)
{
  if (!all(args, argc, &Integer)) {
    return EX("all args should be integer.");
  } else {
    return Integer.sum(meta, args, argc);
  }
}


void BindPF(Generator* g, Object* env)
{
  Env.bind(env, g->symbol(g->meta_obj, "+"),
	   PrimFunc.new(g->meta_obj, "+", sum_int));
}
