#include "stack.h"


int main( void )
{
  Stack<int> stk = {};
  if (!StackInit(&stk))
    Stack_Process_Error(&stk);
  else
    StackProcLoop(&stk);
  StackClose(&stk);

  return 0;
}