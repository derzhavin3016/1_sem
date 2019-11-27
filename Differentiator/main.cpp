#include "Diff.h"

using namespace ad6;

int main( void )
{
  try 
  {
  tree tr;
  tr.process_loop();
  //tr.read_tree("t.txt");
  //printf("%lg\n", tr.tree_calc());
  //tr.dump_root("tata");
  /*tr.tree_diff("x");
  tr.dump_diff("diffx");
  node **df = tr.get_diff();
  tr._simplifier(df);
  tr.dump_diff("diffx_smp");
  tr.tex_dump("go", tr.get_root());*/
 /* tr.dump_diff("tij");
  tr.dump_diff("tji");*/
  int dummy = 0;
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