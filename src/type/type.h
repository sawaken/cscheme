#include <stdbool.h>

#ifndef TYPE_H
#define TYPE_H

#ifndef __cplusplus
#define new New
#endif

#define LEN_INF 100000

typedef struct Object Object;
struct Object
{
  void* type;
  void* data;
  Object* meta_obj;
  int meta_obj_pos;
};

typedef struct
{
  Object* meta_obj;
  Object* (*cons)(Object*, Object*, Object*);
  Object* (*symbol)(Object*, char*);
  Object* (*exception)(Object*, char*);
} Generator;

typedef struct
{
  void (*release)(Object* obj);
  void (*apply)(Object* obj, void (*proc)(Object*));
  void (*onReferred)(Object* obj);
  void (*onUnreferred)(Object* obj);
} Controller;

typedef struct
{
  Controller con;
  Object* (*New)(int init_max_size);
  Object* (*gen)(Object* meta, void* type, void* data);
  void (*referred)(Object* obj);
  void (*unreferred)(Object* obj);
  void (*release)(Object* obj);
  int (*sweep)(Object* obj);
} t_MetaObject;
extern t_MetaObject MetaObject;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, char* name);
  char* (*to_s)(Object* symbol);
} t_Symbol;
extern t_Symbol Symbol;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, int i);
  int (*to_i)(Object* integer);
} t_Integer;
extern t_Integer Integer;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* car, Object* cdr);
  Object* (*car)(Object* cell);
  Object* (*cdr)(Object* cell);
} t_Cell;
extern t_Cell Cell;

typedef struct
{
  Controller con;
  Object* (*New)(Object* mata, Object* env, Object* exp);
  Object* (*pos)(Object* form);
  Object* (*restNum)(Object* form);
  Object* (*next)(Object* form);
  Object* (*command)(Object* form);
  Object* (*evaluatedArg)(Object* form, int pos);
  Object* (*rawArg)(Object* form, int pos);
  Object** (*elements)(Object* form, int start_pos);
  void (*back)(Object* form, Object* obj);
  bool (*hasUnevaluated)(Object* form);
  bool (*commandEvaluated)(Object* form);
  bool (*isBody)(Object* form);
} t_Form;
extern t_Form Form;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* cont);
  Object* (*dup)(Object* cont);
  Object* (*top)(Object* cont);
  void (*pop)(Object* cont);
  void (*push)(Object* cont, Object* obj);
  void (*call)(Object* cont, Object* alt_cont, Object* obj);
  void (*ret)(Object* cont);
  void (*replace)(Object* cont, Object* obj);
  void (*erase)(Object* cont, int start_pos, int n);
  int (*size)(Object* cont);
  Object* (*at)(Object* cont, int pos);
} t_Continuation;
extern t_Continuation Continuation;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, char* name, void (*action)(Object* cont),
		 int len_min, int len_max);
  void (*doAction)(Object* meta, Object* sf, Object* cont);
} t_SpecialForm;
extern t_SpecialForm SpecialForm;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* param_list, 
		 Object** exps, int n);
} t_Lambda;
extern t_Lambda Lambda;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* ref);
  Object* (*ref)(Object* dummy);
  bool (*isReleased)(Object* dummy);
  int (*ref_count)(Object* dummy);
  int (*unref_count)(Object* dummy);
} t_Dummy;
extern t_Dummy Dummy;


bool IsA(Object* obj, void* type);
Controller* Con(Object* obj);

#endif
