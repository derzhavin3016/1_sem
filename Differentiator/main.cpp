#include "Diff.h"

using namespace ad6;

int main( void )
{
  try 
  {
  tree tr;
  tr.read_tree("t.txt");
//  printf("%lg\n", tr.tree_calc());
  tr.dump("tt");
  int dummy = 0;
  }
  catch ( Error &err)
  {
    std::cerr << err.descr << std::endl;
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