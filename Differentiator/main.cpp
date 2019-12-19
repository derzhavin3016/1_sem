
#pragma warning (disable: 4267)
#include "Diff.h"



using namespace ad6;

int main( void )
{
  try 
  {
    char *buf = nullptr, *buf2 = nullptr;
    tree tr;
    tr.process_loop();
    /*buf = strdup(InputAnswer("Input filename to start:"));
    buf2 = InputAnswer("Input file name to write answer:\n");

    double answ = tr.laba_kill(buf);

    printf("The answer is: %lg\n", answ);
    free(buf);
    
    tr.write_answ(buf2, answ);*/
  }
  catch ( Error &err)
  {
    std::cerr << err.descr << " In file: " << err.file << 
                 " line (" << err.line << ") function " << err.func << std::endl;
  }
  catch ( std::bad_alloc &err )
  {
    std::cerr << err.what() << std::endl;
  }
  catch (...)
  {
    printf("Strange throw\n");
  }
  return 0;
}