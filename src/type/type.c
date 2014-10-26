#include <stdbool.h>
#include "type.h"


bool IsA(Object* obj, void* type)
{
  return obj->type == type;
}
