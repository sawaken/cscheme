#ifndef PARSE_UTIL_H
#define PARSE_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

  int StringEndPos(const char str[], int begin, int end, bool escaped);
  int LineEndPos(const char str[], int begin, int end);
  bool hasOnlyDigit(const char str[]);
  bool isIdent(char c);
  int BracketNestOutPosition(const char str[], int nest_level, int begin_pos, int end_pos);
  bool CheckBracket(const char str[], int begin_pos, int end_pos, int countf);
  int WordEndPosition(const char str[], int begin_pos, int end_pos);
  bool isSeparation(char c);
  bool between(char c, char s, char t);
  bool array_member(char c, char array[], int len);

#ifdef __cplusplus
}
#endif

#endif
