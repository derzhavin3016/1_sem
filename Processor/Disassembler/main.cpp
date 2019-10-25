#include "disasm.h"

int main( int argc, char *argv[] )
{
  ad6::Disasm dis;
     
  if (!dis.Disassembly(argv[1], argv[2]))
  {
    printf("ERROR!!!\n");
    return 1;
  }

  return 0;
}