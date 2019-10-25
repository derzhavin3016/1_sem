#include "asm.h"

int main( int argc, char *argv[] )
{
  ad6::Asm asmblr;
  StrConcat(argv[2], ".adasm");
  if (!asmblr.Assembly(argv[1], argv[2]))
    printf("ERROR\n");

  return 0;
}