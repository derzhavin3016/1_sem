#include "Diff.h"

using namespace ad6;

int main( void )
{
  try 
  {
  tree tr;
  tr.read_tree("t.txt");
  tr.dump("tt");
  int dummy = 0;
  }
  catch ( Error &err)
  {
    std::cout << err.descr << std::endl;
  }
  catch ( std::bad_alloc &err )
  {
    std::cout << err.what() << std::endl;
  }
  catch (...)
  {
    printf("Strange throw\n");
  }
  return 0;
}