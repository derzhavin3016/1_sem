#include "stack.h"

int main( void )
{
  Stack<int> stk = {};

  StackInit(&stk);
  StackPush(&stk, 10);
  int val = StackPop(&stk);

  StackDump(&stk, "Just an example", stack_location);

  StackClose(&stk);

  return 0;
}