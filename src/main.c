#include <stdio.h>
#include "type/type.h"
#include "util.h"
#include "cscheme.h"

#define REPL_MODE 1

int main(void)
{
  Interpreter* inter = Cscheme.new();

  if (REPL_MODE) {
    
    char buf[1000];

    while (printf("cscheme> ") && fgets(buf, 1000, stdin) != NULL) {
      Cscheme.eval(inter, buf);
      printf("%s\n", Util.toStr(Cscheme.getEvaluated(inter), buf)); // temporary solution
    }
  }

  Cscheme.release(inter);

  return 0;
}
