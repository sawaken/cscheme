#include <string.h>

#include "object.h"
#include "parse.h"

Object* ParseExp(char exp[], Generator* gen)
{
  return iParseExp(exp, cons, symbol, exception, 0, strlen(exp), MODE_NONE);
}

static Object* iParseExp(char exp[], Generator* gen, int begin, int end, ParseMode mode)
{
  return NULL;
}

