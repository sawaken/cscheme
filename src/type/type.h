#include <stdbool.h>

#ifndef TYPE_H
#define TYPE_H

#ifndef __cplusplus
#define new New
#endif


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
  int (*size)(Object* meta);
  int (*pos)(Object* meta);
  void (*referred)(Object* obj);
  void (*unreferred)(Object* obj);
  void (*release)(Object* obj);
  int (*sweep)(Object* obj);
  Object* (*findSymbol)(Object* meta, const char* name,
			int (*strcmp)(const char* s1,
				      const char* s2));
  
} CSCM_MetaObject_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, const char name[]);
  Object* (*newWithRange)(Object* meta, const char str[], int s, int t);
  char* (*to_s)(Object* symbol);
} CSCM_Symbol_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, int i);
  int (*to_i)(Object* integer);
  Object* (*sum)(Object* meta, Object** ints, int len);
} CSCM_Integer_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* car, Object* cdr);
  Object* (*car)(Object* cell);
  Object* (*cdr)(Object* cell);
  bool (*empty)(Object* cell);
} CSCM_Cell_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* mata, Object* env, Object* exp,
		 int length, bool body);
  int (*pos)(Object* form);
  int (*restNum)(Object* form);
  int (*size)(Object* form);
  Object* (*env)(Object* form);
  Object* (*next)(Object* form);
  Object* (*evaluatedElement)(Object* form, int position);
  Object* (*rawElement)(Object* form, int position);
  Object** (*evaluatedElements)(Object* form, int start_pos);
  Object** (*rawElements)(Object* form, int start_pos);
  void (*back)(Object* form, Object* obj);
  bool (*isBody)(Object* form);
} CSCM_Form_T;

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
} CSCM_Continuation_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, const char* name,
		 bool (*action)(Object* meta, Object* cont));
  bool (*doAction)(Object* sf, Object* meta, Object* cont);
} CSCM_SpecialForm_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* env, Object* param,
		 Object* const exps[], int length);
  Object* (*env)(Object* lambda);
  Object* (*param)(Object* lambda);
  Object** (*exps)(Object* lambda);
  int (*expc)(Object* lambda);
} CSCM_Lambda_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* parent);
  int (*size)(Object* env);
  void (*bind)(Object* env, Object* key, Object* value);
  Object* (*find)(Object* env, Object* key,
		  int (*comp)(Object*, Object*));
} CSCM_Env_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* raised_obj);
  Object* (*take)(Object* exception);
} CSCM_Exception_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, const char str[]);
  Object* (*newWithRange)(Object* meta, const char str[], int s, int t);
  char* (*to_s)(Object* string);
} CSCM_String_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* const params[],
		 int paramc, Object* rest);
  int (*paramc)(Object* param);
  Object** (*params)(Object* param);
  Object* (*rest)(Object* param);
  Object* (*at)(Object* param, int pos);
  bool (*validArgLength)(Object* param, int length);
} CSCM_Parameter_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, const char name[],
		 Object* (*func)(Object*, Object**, int));
  Object* (*apply)(Object* pf, Object* meta,
		   Object** args, int argc);
} CSCM_PrimFunc_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, bool b);
  bool (*to_b)(Object* b);
  Object* (*select)(Object* b, Object* obj1, Object* obj2);
} CSCM_Bool_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, char c);
  char (*to_c)(Object* character);
} CSCM_Character_T;

typedef struct
{
  Controller con;
  Object* (*New)(Object* meta, Object* ref);
  Object* (*ref)(Object* dummy);
  bool (*isReleased)(Object* dummy);
  int (*ref_count)(Object* dummy);
  int (*unref_count)(Object* dummy);
} CSCM_Dummy_T;


extern CSCM_MetaObject_T   CSCM_MetaObject;
extern CSCM_Symbol_T       CSCM_Symbol;
extern CSCM_Integer_T      CSCM_Integer;
extern CSCM_Cell_T         CSCM_Cell;
extern CSCM_Form_T         CSCM_Form;
extern CSCM_Continuation_T CSCM_Continuation;
extern CSCM_SpecialForm_T  CSCM_SpecialForm;
extern CSCM_Lambda_T       CSCM_Lambda;
extern CSCM_Env_T          CSCM_Env;
extern CSCM_Exception_T    CSCM_Exception;
extern CSCM_String_T       CSCM_String;
extern CSCM_Parameter_T    CSCM_Parameter;
extern CSCM_PrimFunc_T     CSCM_PrimFunc;
extern CSCM_Bool_T         CSCM_Bool;
extern CSCM_Character_T    CSCM_Character;
extern CSCM_Dummy_T        CSCM_Dummy;


#ifdef __cplusplus
extern "C" {
#endif

  bool IsA(Object* obj, void* type);

#ifdef __cplusplus
}
#endif

#endif
