#include "processor.h"

int main( void )
{
  ad6Proc prc;

  if (!prc.Execute("b.adasm"))
    printf("ERROR");
  prc.stk.Dump("check", STACK_LOCATION);
  return 0;
}