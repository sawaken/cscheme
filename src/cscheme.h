#include "type/type.h"

#ifndef CSCHEME_H
#define CSCHEME_H

typedef struct
{
  Object* meta;
  Object* top_env;
  Object* evaluated;
} CSCM_Interpreter;

typedef struct
{
  CSCM_Interpreter* (*New)(void);
  void (*eval)(CSCM_Interpreter* inter, const char code[]);
  void (*ret)(CSCM_Interpreter* inter, Object* evaluated);
  Object* (*getEvaluated)(CSCM_Interpreter* inter);
  void (*release)(CSCM_Interpreter* inter);
  char* (*showLastEvaluated)(CSCM_Interpreter * inter, char* buf);
} CSCM_T;
extern CSCM_T CSCM;

#endif
