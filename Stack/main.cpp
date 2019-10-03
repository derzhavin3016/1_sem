#include "stack.h"


int main( void )
{
  Stack<int> stk = {};
  StackInit(&stk);
  StackProcLoop(&stk);
  StackClose(&stk);

  return 0;
}