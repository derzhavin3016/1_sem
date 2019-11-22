#include "par.h"

using namespace ad6;

#pragma warning (disable: 4996)

int main( void )
{
  parser pr;

  char str[100];

  scanf("%s", str);

  printf("%d\n", pr.getG(str));

  return 0;
}