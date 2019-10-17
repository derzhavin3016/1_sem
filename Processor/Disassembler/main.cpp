#include "disasm.h"

int main( int argc, char *argv[] )
{
  ad6Disasm dis;
  /*char file_name_in[100] = {},
    file_name_out[100] = {};

  if (argc == 1)
  {
    int IsOK = Input("# Disassembly from file programn\n\n"
      "Input file name to start (*.adasm): ", "%s", file_name_in);
    if (!IsOK)
    {
      printf("Incorrect file name to read.\n");
      return 1;
    }

    IsOK = Input("Input file name to write: ", "%s", file_name_out);
    if (!IsOK)
    {
      printf("Incorrect file name to write.\n");
      return 1;
    }
  }*/
    
  if (!dis.Disassembly(argv[1], argv[2]))
  {
    printf("ERROR!!!\n");
    return 1;
  }

  return 0;
}