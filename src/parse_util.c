#include <stdbool.h>
#include "parse_util.h"

static char SPECIAL_ID[] = {'!', '$', '%', '&', '*', '+', '-', '.',
			    '/', '<', '=', '>', '?', '@', '^', '_'};

int StringEndPos(const char str[], int begin, int end, bool escaped)
{
  if( begin >= end )
    return -1;
  if(escaped)
    return StringEndPos(str, begin + 1, end, false);
  if( str[begin] == '"' )
    return begin;
  if( str[begin] == '\\' )
    return StringEndPos(str, begin + 1, end, true);
  else
    return StringEndPos(str, begin + 1, end, false);
}

int LineEndPos(const char str[], int begin, int end)
{
  if( begin >= end )
    return end;
  if( str[begin] == '\n' || str[begin] == '\0' )
    return begin;
  else
    return LineEndPos(str, begin + 1, end);
}

bool hasOnlyDigit(const char str[])
{
  for(int i = 0; str[i] != '\0'; i++)
    if(str[i] < '0' || '9' < str[i])
      return false;
  return true;
}

bool isIdent(char c)
{
  if( between(c, '0', '9') || between(c, 'A', 'Z') || between(c, 'a', 'z') )
    return true;
  else if(array_member(c, SPECIAL_ID, 16))
    return true;
  else
    return false;
}

int BracketNestOutPosition(const char str[], int nest_level, int begin_pos, int end_pos)
{
  if(nest_level == 0){
    return begin_pos;
  }else if(begin_pos == end_pos){
    return -1;
  }else if(str[begin_pos] == '('){
    return BracketNestOutPosition(str, nest_level + 1, begin_pos + 1, end_pos);
  }else if(str[begin_pos] == ')'){
    return BracketNestOutPosition(str, nest_level - 1, begin_pos + 1, end_pos);
  }else{
    return BracketNestOutPosition(str, nest_level, begin_pos + 1, end_pos);
  }
}

bool CheckBracket(const char str[], int begin_pos, int end_pos, int count)
{
  if( begin_pos == end_pos )
    return count == 0;
  if( str[begin_pos] == '(')
    return CheckBracket(str, begin_pos + 1, end_pos, count + 1);
  if( str[begin_pos] == ')')
    return count == 0 ? false : CheckBracket(str, begin_pos + 1, end_pos, count - 1);
  else
    return CheckBracket(str, begin_pos + 1, end_pos, count);
}

int WordEndPosition(const char str[], int begin_pos, int end_pos)
{
  if(begin_pos == end_pos){
    return end_pos;
  }else if( isSeparation(str[begin_pos]) ){
    return begin_pos;
  }else{
    return WordEndPosition(str, begin_pos + 1, end_pos);
  }
}

bool isSeparation(char c)
{
  return c == ' ' || c == '\n' || c == '\t';
}

bool between(char c, char s, char t)
{
  return s <= c && c <= t;
}

bool array_member(char c, char array[], int len)
{
  for(int i = 0; i < len; i++)
    if(array[i] == c)
      return true;
  return false;
}
