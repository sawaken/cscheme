#include <stdlib.h>
#include "type.h"

Object* New(void* type, void* data)
{
  Object* object = malloc(sizeof(Object));
  object->type = type;
  object->data = data;
  
  Con(object)->apply(object, Referred);
  return object;
}

bool IsA(Object* obj, void* type)
{
  return obj->type == type;
}

Controller* Con(Object* obj)
{
  return (Controller*)(obj->type);
}

void Referred(Object* obj)
{
  Controller* c = Con(obj);

  if (c->referred != NULL) {
    c->referred(obj);
  }

  obj->ref_count++;
}

void Unreferred(Object* obj)
{
  Controller* c = Con(obj);

  if (c->unreferred != NULL) {
    c->unreferred(obj);
  }

  obj->gcd.unref_count++;

  if (obj->gcd.ref_count == obj->gcd.unref_count) {
    Release(obj);
  }
}

void Release(Object* obj)
{
  Controller* c = Con(obj);
  
  if (obj->gcd.death_flag) {
    return 0;
  } else {
    obj->gcd.death_flag = 1;
  }

  if (c->apply != NULL) {
    c->apply(obj, Unreferred);
  }

  if (c->release != NULL) {
    c->release(obj);
  }

  release(obj->data);
  release(obj);
}



//debug.h

typedef struct
{
  Controller con;
  Object* obj;
} Mock;

typedef struct
{
  int serial_count;
  

} Debugger;

typedef struct
{
  Debugger* debugger;
  int serial_number;
} DebugInfo;



typedef struct


//debug.c

static int object_serial;

