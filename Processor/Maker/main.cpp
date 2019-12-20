#include "process.h"

int main( void )
{
  try
  {
    ProcessLoop();
  }
  catch ( Error &err )
  {
    std::cerr << "ERROR!!!\n" << err.descr << "\nIn file: " << err.file
              << " in function "<< err.func << " line (" << err.line << ")\n";
  }
  catch ( int err )
  {
    std::cerr << "\nError with stack ";
  }
  catch (...)
  {
    std::cerr << "Strange error";
  }

  return 0;
}