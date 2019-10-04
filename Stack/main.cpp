#include "stack.h"


struct Tortue
{
  int exec1;
  Stack<int> victim;
  int exec2;
};

int main( void )
{
  Tortue trt = {};
  StackInit(&trt.victim);
  *((canary_t *)((int *)&trt.exec1 + 2)) = 25;
  //StackProcLoop(&stk);
  //StackDump(&stk, "check", STACK_LOCATION);
  StackClose(&trt.victim);
  
  return 0;
}