#include "Akinator.h"

using namespace ad6;

int main( void )
{
  Aki ak;
  ak.ReadTree("tree1.txt");
  ak.ProcessLoop();
  ak.SaveTree("tree2.txt");

  return 0;
}