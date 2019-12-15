#include "Frontend\Lng.h"
#include "Frontend^-1/frontback.h"

using namespace ad6;

int main( void )
{
  try
  {
    frontend fr;
    fback fb;
    fr.read_tree("Programs\\fact.txt");
    fr.dump_root("fact");
    fr.save_tree("fac.txt");
    fb.read_tree("fac.txt");
    fb.dump_root("ff");
  }
  catch ( Error &err)
  {
    std::cerr << err.descr << "\n In file: " << err.file << 
                 " line (" << err.line << ") function " << err.func << std::endl;
  }
  catch ( std::bad_alloc &err )
  {
    std::cerr << err.what() << std::endl;
  }
  catch ( syntax_error &err )
  {
    std::cerr << "SYNTAX ERROR!!!!:\n"
      << err.descr << "\n line (" << err.line << ") position " << err.pos << std::endl;
  }
  catch (...)
  {
    printf("Strange throw\n");
  }
  return 0;
}