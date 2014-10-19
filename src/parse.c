#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "type/type.h"
#include "parse_util.h"
#include "parse.h"


enum ParseMode{
  PMODE_NONE,
  PMODE_SINGLE,
  PMODE_BACK,
  PMODE_COMMA,
  PMODE_COMMA_AT,
  PMODE_DOUBLE,
  PMODE_SINGLE_SHARP,
  PMODE_COLLECT_SINGLE,
  PMODE_SINGLE_SHARP_BSLASH
};
typedef enum ParseMode ParseMode;

static Object* stop(const char* msg)
{
  printf("%s\n", msg);
  exit(1);
}

static Object* StringParse(const char str[], Generator* gen, int begin, int end);
static Object* Quoten(Object* exp, Generator* gen, ParseMode mode);
static Object* ParseIdent(const char exp_str[], Generator* gen, int begin, int end);
static Object* iParseExp(const char exp[], Generator* gen, int begin, int end, ParseMode mode);

Object* ParseExp(const char exp[], Generator* gen)
{
  return iParseExp(exp, gen, 0, strlen(exp), PMODE_NONE);
}

static Object* iParseExp(const char exp[], Generator* gen, int begin, int end, ParseMode mode)
{
  if( begin >= end )
    return gen->cons(gen->meta_obj, NULL, NULL);

  if( mode != PMODE_DOUBLE && mode != PMODE_SINGLE_SHARP_BSLASH && isSeparation(exp[begin]) )
    return iParseExp(exp, gen, begin + 1, end, mode);

  if( mode != PMODE_DOUBLE && mode != PMODE_SINGLE_SHARP_BSLASH && exp[begin] == ';' )
    return iParseExp(exp, gen, LineEndPos(exp, begin, end) + 1, end, mode);

  if( mode == PMODE_NONE && exp[begin] == '\'' )
    return iParseExp(exp, gen, begin + 1, end, PMODE_SINGLE);

  if( mode == PMODE_NONE && exp[begin] == '"' )
    return iParseExp(exp, gen, begin + 1, end, PMODE_DOUBLE);

  if( mode == PMODE_NONE && exp[begin] == '`' )
    return iParseExp(exp, gen, begin + 1, end, PMODE_BACK);

  if( mode == PMODE_NONE && exp[begin] == ',' )
    return iParseExp(exp, gen, begin + 1, end, PMODE_COMMA);

  if( mode == PMODE_COMMA && exp[begin] == '@' )
    return iParseExp(exp, gen, begin + 1, end, PMODE_COMMA_AT);

  if( mode == PMODE_SINGLE && exp[begin] == '#' )
    return iParseExp(exp, gen, begin + 1, end, PMODE_SINGLE_SHARP);

  if( mode == PMODE_COLLECT_SINGLE && exp[begin] == '\'' )
    return iParseExp(exp, gen, begin + 1, end, PMODE_NONE);

  if( mode == PMODE_SINGLE_SHARP ){
    if( exp[begin] == 't' || exp[begin] == 'f' )
      return gen->cons(gen->meta_obj, Bool.new(gen->meta_obj, exp[begin] == 't'),
		       iParseExp(exp, gen, begin + 1, end, PMODE_COLLECT_SINGLE));

    if( exp[begin] == '\\' )
      return iParseExp(exp, gen, begin + 1, end, PMODE_SINGLE_SHARP_BSLASH);

    else
      return stop("Syntax Error: Invalid character after \"'#\".");
  }

  if( mode == PMODE_SINGLE_SHARP_BSLASH )
    return gen->cons(gen->meta_obj, Character.new(gen->meta_obj, exp[begin]),
		     iParseExp(exp, gen, begin + 1, end, PMODE_COLLECT_SINGLE));

  if( mode == PMODE_DOUBLE ){
    int str_end_pos = StringEndPos(exp, begin, end, false);

    if( str_end_pos < 0 )
      return stop("Syntax Error: Invalid string literal.");

    else
      return gen->cons(gen->meta_obj, Quoten(StringParse(exp, gen, begin, str_end_pos),
					     gen, PMODE_SINGLE),
		       iParseExp(exp, gen, str_end_pos + 1, end, PMODE_NONE));
  }

  if( exp[begin] == '(' ){
    int position = BracketNestOutPosition(exp, 1, begin + 1, end);
    return gen->cons(gen->meta_obj, Quoten(iParseExp(exp, gen, begin + 1, position - 1, PMODE_NONE),
					   gen, mode),
		     iParseExp(exp, gen, position, end, PMODE_NONE));
  }

  if( isIdent(exp[begin]) ){
    int position = WordEndPosition(exp, begin, end);
    return gen->cons(gen->meta_obj, Quoten(ParseIdent(exp, gen, begin, position), gen, mode),
		     iParseExp(exp, gen, position, end, PMODE_NONE));
  }

  else
    return stop("Syntax Error: Invalid token.");
}
 

static Object* StringParse(const char str[], Generator* gen, int begin, int end)
{
  return String.newWithRange(gen->meta_obj, str, begin, end);
}

static Object* Quoten(Object* exp, Generator* gen, ParseMode mode)
{
  if(mode == PMODE_NONE)
    return exp;

  if(mode == PMODE_SINGLE)
    return gen->cons(gen->meta_obj, gen->symbol(gen->meta_obj, "quote"), 
		     gen->cons(gen->meta_obj, exp, NULL));

  if(mode == PMODE_BACK)
    return gen->cons(gen->meta_obj, gen->symbol(gen->meta_obj, "quasiquote"),
		     gen->cons(gen->meta_obj, exp, NULL));

  if(mode == PMODE_COMMA)
    return gen->cons(gen->meta_obj, gen->symbol(gen->meta_obj, "unquote"),
		     gen->cons(gen->meta_obj, exp, NULL));

  if(mode == PMODE_COMMA_AT)
    return gen->cons(gen->meta_obj, gen->symbol(gen->meta_obj, "unquote-splicing"),
		     gen->cons(gen->meta_obj, exp, NULL));
  else
    return stop("Syntax Error: Invalid quoting.");
}

static Object* ParseIdent(const char exp_str[], Generator* gen, int begin, int end)
{
  char name_buf[101];

  for(int i = begin; i < end; i++)
    name_buf[i - begin] = exp_str[i];

  name_buf[end - begin] = '\0';

  if( hasOnlyDigit(name_buf) )
    return Integer.new(gen->meta_obj, atoi(name_buf));
  else
    return gen->symbol(gen->meta_obj, name_buf);
}
