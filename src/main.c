#include <stdio.h>
#include "cscheme.h"

#define REPL_MODE 1

int main(void)
{
  CSCM_Interpreter* inter = CSCM.New();

  if (REPL_MODE) {
    
    char buf[1000];

    while (fputs("cscheme> ", stdout) && fgets(buf, 1000, stdin) != NULL) {
      CSCM.eval(inter, buf);
      printf("%s\n", CSCM.showLastEvaluated(inter, buf));
    }

    fputs("\n", stdout);
  }


  CSCM.release(inter);

  return 0;
}
