#include <stdbool.h>

#ifndef TYPE_H
#define TYPE_H

#ifndef __cplusplus
#define new New
#endif

#define LEN_INF 100000
#define pull(obj) (assert((obj)->type == Type), (Data*)((obj)->data))

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
  Object* (*symbol)(Object*, const char[]);
  Object* (*exception)(Object*, char*);
} Generator;

typedef struct
{
  bool (*release)(Object* obj);
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
  Object* (*New)(Object* meta, const char name[]);
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
  bool (*empty)(Object* cell);
} t_Cell;
extern t_Cell Cell;

typedef struct
{
  Controller con;
  Object* (*New)(Object* mata, Object* env, Object* exp,
		 int length, bool body);
  int (*pos)(Object* form);
  int (*restNum)(Object* form);
  Object* (*env)(Object* form);
  Object* (*next)(Object* form);
  Object* (*evaluatedElement)(Object* form, int position);
  Object* (*rawElement)(Object* form, int position);
  Object** (*evaluatedElements)(Object* form, int start_pos);
  Object** (*rawElements)(Object* form, int start_pos);
  void (*back)(Object* form, Object* obj);
  bool (*isBody)(Object* form);
} t_Form;
extern t_Form Form;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* init_cont);
  Object* (*top)(Object* cont);
  int (*size)(Object* cont);
  int (*max_size)(Object* cont);
  Object* (*at)(Object* cont, int pos);
  void (*erase)(Object* cont, int start_pos, int length);
  void (*pop)(Object* cont);
  void (*push)(Object* cont, Object* obj);
  void (*trans)(Object* cont, Object* alt_cont, Object* obj);
  void (*returnTopToForm)(Object* cont);
  void (*popAndPush)(Object* cont, Object* obj);
} t_Continuation;
extern t_Continuation Continuation;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, const char* name,
		 bool (*action)(Object* cont),
		 int len_min, int len_max);
  bool (*validArgc)(Object* sf, int argc);
  bool (*doAction)(Object* sf, Object* meta, Object* cont);
} t_SpecialForm;
extern t_SpecialForm SpecialForm;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* param,
		 Object* const exps[], int length);
  Object* (*param)(Object* lambda);
  Object** (*exps)(Object* lambda);
  int (*expc)(Object* lambda);
} t_Lambda;
extern t_Lambda Lambda;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* parent);
  int (*size)(Object* env);
  void (*bind)(Object* env, Object* key, Object* value);
  Object* (*find)(Object* env, Object* key,
		  bool (*comp)(Object*, Object*));
} t_Env;
extern t_Env Env;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* raised_obj);
  Object* (*take)(Object* exception);
} t_Exception;
extern t_Exception Exception;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, const char str[]);
  char* (*to_s)(Object* string);
} t_String;
extern t_String String;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* const params[],
		 int paramc, Object* rest);
  int (*paramc)(Object* param);
  Object** (*params)(Object* param);
  Object* (*rest)(Object* param);
  Object* (*at)(Object* param, int pos);
} t_Parameter;
extern t_Parameter Parameter;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, const char name[],
		 Object* (*func)(Object**, int));
  Object* (*apply)(Object* pf, Object* meta,
		   Object** args, int argc);
} t_PrimFunc;
extern t_PrimFunc PrimFunc;

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
