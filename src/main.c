#include <stdio.h>
#include "cscheme.h"

#define REPL_MODE 1

int main(void)
{
  Interpreter* inter = Cscheme.New();

  if (REPL_MODE) {
    
    char buf[1000];

    while (printf("cscheme> ") && fgets(buf, 1000, stdin) != NULL) {
      Cscheme.eval(inter, buf);
      printf("%s\n", Cscheme.showLastEvaluated(inter, buf));
    }
  }

  Cscheme.release(inter);

  return 0;
}
