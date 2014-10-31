
typedef struct
{
  int (*extractSymbols)(Object* pattern_variables, Object* template, Object* buf[], int buf_size);
  Object* (*convertTemplate)(Object* meta, Object* pattern_variables, Object* pattern_assoc,
			     Object* freeSymbols[], Object* renamedSymbols[],
			     int symbols_len, Object* template);
  bool (*patternVariableMatch)(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env);
  bool (*literalMatch)(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env);
  bool (*listMatch)(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env);
  bool (*nonAuthenticListMatch)(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env);
  bool (*ellipsisListMatch)(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env);
  bool (*match)(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env);
  bool (*matchList)(Object* literals, Object* pattern_list, Object* pattern_env,
		    Object* exp_list, Object* exp_env, int len);
  bool (*matchAll)(Object* literals, Object* pattern, Object* pattern_env, Object* exp_list, Object* exp_env);
  bool (*isEllipsisSymbol)(Object* obj);
} CSCM_PL_T;

extern CSCM_PL_T CSCM_PL;
