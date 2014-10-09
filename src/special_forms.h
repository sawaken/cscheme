#include "type/type.h"

typedef struct
{
  int n;
  Object** sfs;
} SFList;

SFList SFListGen(Object* meta);
