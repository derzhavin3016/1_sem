#include "par.h"

using namespace ad6;

#pragma warning (disable: 4996)

int main( void )
{
  parser pr;

  char str[100];

  scanf("%s", str);
  try
  {
  printf("%d\n", pr.getG(str));
  }
  catch( Error &err )
  {
    std::cerr << err.descr << " \nfile" << err.file << "in function" <<
      err.func << "in line " << err.line << std::endl;
  }
  catch(...)
  {
    std::cerr << "Strange throw" << std::endl;
  }
  return 0;
}