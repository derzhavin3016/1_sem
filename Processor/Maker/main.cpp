#include "..\Assembler\asm.h"
#include "..\Disassembler\disasm.h"
#include "..\Proc\processor.h"
#define NDEBUG

using namespace ad6;

int main( void )
{
  Asm as;
  Disasm dis;
  Proc cp;
  
#define IN_F "lng\\square_asm.txt"
#define OUT_F "lng\\square.adasm"
#define DIS_F "lng\\square.txt"
  
  
  if (!as.Assembly(IN_F, OUT_F))
    return false;
  if (!dis.Disassembly(OUT_F, DIS_F))
    return false;
    try
    {
      cp.Execute(OUT_F);
    }
    catch (...)
    {
      printf("Stack error\n");
    }
  return 0;
}