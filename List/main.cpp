#include "List.h"
#include <Windows.h>

using namespace ad6;

int main( void )
{
  List<int> lst;
  try
  {
    ListProcLoop(&lst);
  }
  catch (...)
  {
    printf("ERROR!!!\n");
  }

  return 0;
}