#ifndef PCHEME_H
#define PCHEME_H

typedef struct
{
  Object* meta;
  Object* top_env;
  Object* evaluated;
} Interpreter;

typedef struct
{
  Interpreter* (*New)(void);
  void (*eval)(Interpreter* inter, const char code[]);
  void (*ret)(Interpreter* inter, Object* evaluated);
  Object* (*getEvaluated)(Interpreter* inter);
  void (*release)(Interpreter* inter);
} t_Pcheme;
extern t_Pcheme Pcheme;

#endif
