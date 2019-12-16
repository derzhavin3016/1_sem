#include "Frontend\Lng.h"
#include "Frontend^-1/frontback.h"

using namespace ad6;

int main( void )
{
  try
  {
    frontend fr;
    fback fb;
    //fr.par_tree("Programs\\square.txt", "sq.txt");
    //fr.dump_root("sqrt");
    fb.tree_to_code("serega.txt", "ser.txt");
    fb.dump_root("ser");
    //fb.tree_to_code("fac.txt", "fer.txt");
    //fb.dump_root("fer");
    //fr.par_tree("drt.txt", "d.txt");
    //fr.dump_root("ff0");
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