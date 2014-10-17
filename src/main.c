#include <stdio.h>
#include "type/type.h"
#include "pcheme.h"

#define REPL_MODE 1

int main()
{
  Interpreter* inter = Interpreter.new();

  if (REPL_MODE) {
    
    char buf[1000];

    while (fgets(buf, 1000, stdion) != NULL) {
      Pcheme.eval(inter, buf);
      printf("%s\n", ToStr(Pcheme.getEvaluated(inter)));
    }
  }

  Pcheme.release(inter);

  return 0;
}
