#include <stdio.h>

#include "type/type.h"
#include "type/type_alias.h"
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

def(meta_info)
{
  if (argc != 0) {
    return EX("invalid args.");
  }

  char buf[100];
  snprintf(buf, sizeof(buf), "[meta: size = %d, pos = %d]",
	   MetaObject.size(meta), MetaObject.pos(meta));

  return String.new(meta, buf);
}


#define BIND(meta, getSymbol, env, func, name) Env.bind((env), (getSymbol)((meta), name), \
							PrimFunc.new((meta), (name), (func)))

void CSCM_BindPF(Object* meta, Object* (*getSymbol)(Object*, const char*), Object* env)
{
  BIND(meta, getSymbol, env, sum_int, "+");
  BIND(meta, getSymbol, env, display, "display");
  BIND(meta, getSymbol, env, meta_info, "meta-info");
}
