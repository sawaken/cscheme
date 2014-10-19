#include <stdbool.h>
#include <stdio.h>
#include "type.h"


bool IsA(Object* obj, void* type)
{
  return obj->type == type;
}

Controller* Con(Object* obj)
{
  return (Controller*)(obj->type);
}

// temporary implimentation
char* ToStr(Object* obj, char buf[])
{
  if (IsA(obj, &Integer)) {
    snprintf(buf, 100, "%d", Integer.to_i(obj));
  } else if (IsA(obj, &Symbol)) {
    snprintf(buf, 100, "<#symbol: %s>", Symbol.to_s(obj));
  } else if (IsA(obj, &String)) {
    snprintf(buf, 100, "<#string: %s>", String.to_s(obj));
  } else if (IsA(obj, &Exception)) {
    snprintf(buf, 100, "<#exception: %s>", String.to_s(Exception.take(obj)));
  } else {
    snprintf(buf, 100, "<#object>");
  }

  return buf;
}
