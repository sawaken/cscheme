#include <stdlib.h>
#include "type.h"

Object* New(void* type, void* data)
{
  Object* object = malloc(sizeof(Object));
  object->type = type;
  object->data = data;
  return object;
}

Controller* Con(Object* obj)
{
  return (Controller*)obj->type;
}
