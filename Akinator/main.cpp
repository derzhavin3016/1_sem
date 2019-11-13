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
  catch ( Error &error )
  {
    std::cerr << error.descr << " In file: " << error.file << 
                 " line (" << error.line << ") function " << error.func << std::endl;
  }
  return 0;
}