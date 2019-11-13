#include "Akinator.h"

using namespace ad6;

int main( void )
{
  Aki ak;
  try
  {
    ak.ProcessLoop();
  }
  catch( std::bad_alloc &error )
  {
    std::cerr << error.what() << std::endl;
  }
  return 0;
}