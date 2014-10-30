
static bool match(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env)
{
  if (patternVariableMatch(literals, pattern, pattern_env, exp, exp_env))
    return true;

  if (literalMatch(literals, pattern, pattern_env, exp, exp_env))
    return true;

  if (listMatch(literals, pattern, pattern_env, exp, exp_env))
    return true;

  if (nonAuthenticListMatch(literals, pattern, pattern_env, exp, exp_env))
    return true;

  if (ellipsisListMatch(literals, pattern, pattern_env, exp, exp_env))
    return true;

}

static bool patternVariableMatch(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env)
{
  return Util.isA(pattern, &Symbol) && !Util.include(literals, pattern, Util.comp)
}

static bool literalMatch(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env)
{
  if (Util.isA(pattern, &Symbol) && Util.isA(exp, &Symbol) && Util.include(literals, pattern, Util.comp)) {
    Object* resolution1 = Env.find(pattern_env, pattern, Util.comp);
    Object* resolution2 = Env.find(exp_env, exp, Util.comp);

    if (Util.comp(resolution1, resolution2) == 0)
      return true;

    if (Util.comp(pattern, exp) == 0 && resolution1 == NULL && resolution2 == NULL)
      return true;
  }
  return false;
}

static bool listMatch(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env)
{
  return Util.isList(pattern) && Util.isList(exp) &&
    !isEllipsisSymbol(Util.last(pattern)) && matchList(literals, pattern, pattern_env, exp, exp_env, -1);
}

static bool nonAuthenticListMatch(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env)
{
  if (Util.isNonAuthenticList(pattern) && (Util.isList(exp) || Util.isNonAuthenticList(exp))) {
    int n = Util.length(pattern);

    if (Util.length(exp) >= n && matchList(literals, pattern, pattern_env, exp, exp_env, n) &&
	match(literals, Util.ith(pattern, n), pattern_env, Util.drop(n, exp), exp_env))
      return true;
  }
  return fale;
}

static bool ellipsisListMatch(Object* literals, Object* pattern, Object* pattern_env, Object* exp, Object* exp_env)
{
  if (Util.isList(pattern) && Util.isList(exp)) {
    int n = Util.length(pattern) - 2;

    if (n >= 2 && isEllipsisSymbol(Util.ith(pattern, n + 1)) && 
	matchList(literals, pattern, pattern_env, exp, exp_env, n) &&
	matchAll(literals, Util.ith(pattern, n), pattern_env, Util.drop(n, exp), exp_env))
      return true;
  }
  return false;
}

static bool matchList(Object* literals, Object* pattern_list, Object* pattern_env,
		      Object* exp_list, Object* exp_env, int len)
{
  if (len == 0)
    return true;

  if (Cell.empty(pattern_list) && Cell.empty(exp_list))
    return len < 0;

  if (Cell.empty(pattern_list))
    return false;

  if (Cell.empty(exp_list))
    return false;

  return match(literals, Cell.car(pattern_list), pattern_env, Cell.car(exp_list), exp_env)
    && matchList(literals, Cell.cdr(pattern_list), pattern_env, Cell.cdr(exp_list), exp_env, len - 1);
}

static bool matchAll(Object* literals, Object* pattern, Object* pattern_env, Object* exp_list, Object* exp_env)
{
  if (Cell.empty(exp_list))
    return true;
  
  return match(literals, pattern, pattern_env, Cell.car(exp_list), exp_env)
    && matchAll(literals, pattern, pattern_env, Cell.cdr(exp_list), exp_env);
}


static bool isEllipsisSymbol(Object* obj)
{
  return Util.isA(obj, &Symbol) && strcmp(Symbol.to_s(obj), "...") == 0;
}
