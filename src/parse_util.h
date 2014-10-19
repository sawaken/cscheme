#ifndef PARSE_UTIL_H
#define PARSE_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

  bool hasOnlyDigit(char str[]);
  bool isIdent(char c);
  int BracketNestOutPosition(char str[], int nest_level, int begin_pos, int end_pos);
  bool CheckBracket(char str[], int begin_pos, int end_pos, int countf);
  int WordEndPosition(char str[], int begin_pos, int end_pos);
  bool isSeparation(char c);
  bool between(char c, char s, char t);
  bool array_member(char c, char array[], int len);

#ifdef __cplusplus
}
#endif

#endif
