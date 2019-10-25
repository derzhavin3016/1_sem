#include "processor.h"

int main( int arcgc, char *argv[] )
{
  ad6::Proc prc;

  try
  {
    if (!prc.Execute("b.adasm"))
      printf("ERROR");
  }
  catch (...)
  {
    printf("Stack Error\n");
  }
  return 0;
}