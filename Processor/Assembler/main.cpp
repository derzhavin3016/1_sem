#include "asm.h"

int main( void )
{
  ad6Asm asmblr;
  if (!asmblr.Assembly())
    printf("ERROR\n");

  return 0;
}