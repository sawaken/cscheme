#include <stdarg.h>
#include <stdlib.h>
#include "type/type.h"
#include "list_util.h"

static Object* i_ls(Generator* g, int length, va_list ap)
{
  if (length > 0) {
    Object* a = va_arg(ap, Object*);
    return g->cons(g->meta_obj, a, i_ls(g, length - 1, ap));
  }
  else {
    return g->cons(g->meta_obj, NULL, NULL);
  }
}

static Object* new(Generator* g, int length, ...)
{
  va_list ap;
  va_start(ap, length);
  return i_ls(g, length, ap);
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

ListUtil LS = {
  new, symList
};
