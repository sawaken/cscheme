#include <stdbool.h>
#include "type.h"


bool IsA(Object* obj, void* type)
#define PULL(obj) (assert((obj)->type == Type), (Data*)((obj)->data))
{
  return obj->type == type;
}
