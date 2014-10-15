#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

static void* Type = &Lambda;

typedef struct
{
  
} Data;

static bool release(Object* lambda)
{
  return true;
}

static void apply(Object* lambda, void (*proc)(Object*))
{

}

static Object* new(Object* meta, Object* param_list, 
		   Object** exps, int len)
{


}

static Object* makeForm(Object* meta, Object* env,
			Object** args, int argc)
{


}


t_Lambda Lambda = {
  {release, apply, NULL, NULL},
  new, makeForm
};
