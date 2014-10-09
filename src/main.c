#include <stdio.h>

#include "type/type.h"
#include "parse.h"
#include "eval.h"
#include "special_forms.h"
#include "primitive_functions.h"

int main() {

  Object* sym = Symbol.new("hoge");
  Object* num = Integer.new(3);

  printf("%s\n", Symbol.to_s(sym));
  printf("%d\n", Integer.to_i(num));

  Object* list = Util.list("+", "1", "1");

  Object *a = Mock.new(1), *b = Mock.new(2);

  Object* cell = Cell.new(a, b);

  ASSERT_EQ(1, Mock.ref(a));
  ASSERT_EQ(1, Mock.ref(b));
  ASSERT_EQ(0, Mock.uref(a));
  ASSERT_EQ(0, Mock.uref(b));

  Release(cell);

  ASSERT_EQ(1, Mock.ref(a));
  ASSERT_EQ(1, Mock.ref(b));
  ASSERT_EQ(0, Mock.uref(a));
  ASSERT_EQ(0, Mock.uref(b));



  Object* meta = MetaObject.new(100);
  Generator g = {meta, Cell.new, Symbol.new, Exception.new};
  Object* sf_syms = SFSymbols.new(meta);
  Object* top_env = Env.new(meta, NULL);

  InitSpecialForms(meta);
  TopEnvAssign(meta, top_env, sf_syms);

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


