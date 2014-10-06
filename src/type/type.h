#ifndef TYPE_H
#define TYPE_H


typedef struct
{
  void* type;
  void* data;
} Object;

typedef struct
{
  Object* (*cons)(Object*, Object*);
  Object* (*symbol)(char[]);
  Object* (*exception)(char[]);
} Generator;

typedef struct
{
  void (*release)(Object* obj);
  void (*referred)(Object* obj);
  void (*unreferred)(Object* obj);
} Controller;

typedef struct
{
  Controller con;
  Object* (*new)(char* name);
  char* (*to_s)(Object* symbol);
} t_Symbol;

extern t_Symbol Symbol;


typedef struct
{
  Controller con;
  Object* (*new)(int i);
  int (*to_i)(Object* integer);
} t_Integer;

extern t_Integer Integer;


typedef struct
{
  Controller con;
  Object* (*new)(Object* car, Object* cdr);
} t_Cell;

extern t_Cell Cell;


Object* New(void* type, void* data);
Controller* Con(Object* obj);

#endif
