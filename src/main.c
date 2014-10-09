#include <stdio.h>

#include "type/type.h"
#include "parse.h"
#include "eval.h"
#include "special_forms.h"
#include "primitive_functions.h"

int main()
{
  Object* meta = MetaObject.new(100);
  Generator g = {meta, Cell.new, Symbol.new, Exception.new};
  SFList sf_list = SFListGen(meta);
  Object* top_env = Env.new(meta, NULL);

  InitSpecialForms(meta);
  TopEnvAssign(meta, top_env, sf_list);

  if (REPL_MODE) {

    char* buf = malloc(1000 * sizeof(char));

    while (fgets(buf, 1000, stdin) != NULL) {
      Object* exp = ParseExp(buf, &g);
      Object* cont = Continuation.new(meta, Form.new(meta, top_env, exp));
      Eval(meta, cont);
      Object* str = String.newFromObject(meta, Continuation.top(cont));
      printf("%s\n" String.to_s(str));
    }

    free(buf);
  }

  return 0;
}


