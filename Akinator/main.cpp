#include "Akinator.h"

using namespace ad6;

int main( void )
{
  Aki ak;
  ak.ReadTree("tree.txt");
  ak.ProcessLoop();
  ak.SaveTree("tree1.txt");

  return 0;
}