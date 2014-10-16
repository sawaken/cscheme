#include <stdarg.h>
#include <stdlib.h>
#include "type/type.h"
#include "util.h"

static Object* i_list(Generator* g, int length, va_list ap)
{
  if (length > 0) {
    Object* a = va_arg(ap, Object*);
    return g->cons(g->meta_obj, a, i_list(g, length - 1, ap));
  }
  else {
    return g->cons(g->meta_obj, NULL, NULL);
  }
}

static Object* list(Generator* g, int length, ...)
{
  va_list ap;
  va_start(ap, length);
  return i_list(g, length, ap);
}

static Object* i_symList(Generator* g, int length, va_list ap)
{
  if (length > 0) {
    Object* a = g->symbol(g->meta_obj, va_arg(ap, char*));
    return g->cons(g->meta_obj, a, i_symList(g, length - 1, ap));
  }
  else {
    return g->cons(g->meta_obj, NULL, NULL);
  } 
}

static Object* symList(Generator* g, int length, ...)
{
  va_list ap;
  va_start(ap, length);
  return i_symList(g, length, ap);
}

t_Util Util = {
  list, symList
};
