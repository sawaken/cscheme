#include <string.h>

#include "object.h"
#include "parse.h"

Object* ParseExp(char exp[],
		 Object* (*cons)(Object*, Object*),
		 Object* (*symbol)(char[]),
		 Object* (*exception)(char[]))
{
  return iParseExp(exp, cons, symbol, exception, 0, strlen(exp), MODE_NONE);
}

static Object* iParseExp(char exp[],
			 Object* (*cons)(Object*, Object*),
			 Object* (*symbol)(char[]),
			 Object* (*exception)(char[]),
			 int begin,
			 int end,
			 ParseMode mode)
{
  return NULL;
}

