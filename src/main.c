#include <stdio.h>

#include "type/type.h"

int main() {
  Object* sym = Symbol.new("hoge");
  printf("%s\n", Symbol.to_s(sym));

}
