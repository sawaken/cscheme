#include <stdio.h>
#include "type/type.h"
#include "pcheme.h"

#define REPL_MODE 1

int main(void)
{
  Interpreter* inter = Pcheme.new();

  if (REPL_MODE) {
    
    char buf[1000];

    while (printf("pcheme> ") && fgets(buf, 1000, stdin) != NULL) {
      Pcheme.eval(inter, buf);
      printf("%s\n", ToStr(Pcheme.getEvaluated(inter), buf)); // temporary solution
    }
  }

  Pcheme.release(inter);

  return 0;
}
