#include "List.h"
#include <Windows.h>

using namespace ad6;

int main( void )
{
  List<int> lst;
  
  ListProcLoop(&lst);

  return 0;
}