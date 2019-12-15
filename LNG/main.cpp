#include "Frontend\Lng.h"

using namespace ad6;

int main( void )
{
  try
  {
    tree tr;
    tr.read_tree("fact.txt");
    tr.dump_root("fact");
    tr.save_tree("fc.txt");
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