#include <stdio.h>

#include "type/type.h"

int main() {
  Object* sym = Symbol.new("hoge");
  Object* num = Integer.new(3);

  printf("%s\n", Symbol.to_s(sym));
  printf("%d\n", Integer.to_i(num));

  return 0;
}
