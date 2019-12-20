#include "process.h"

/**
 * \brief Process loop function.
 */
void MainProcLoop( void )
{
#define TRS prg[val - 1]->translate(buff, buf1); \
            last = val;                      \
            delete[] buff;

  frontend fr_end;
  fback fr_min;
  backend back;
  mid middle;
  char *buff = nullptr, *buf1 = nullptr;
  int val = 0, last = 0;
  base_translator *prg[4] = {&fr_end, &fr_min, &back, &middle};

  printf(GREETING);
  while (1)
  {
    printf("Input a number to start: ");
    scanf("%d", &val);
    switch (val)
    {
    case 0:
      printf(PARTING);
      return;
    case 1:
      printf("FRONTEND\n");
      buff = strdup(InputAnswer("Input file name to read code from: "));
      buf1 = InputAnswer("Input file name to save tree to: ");
      TRS;
      break;
    case 2:
      printf("FRONTEND ^ -1\n");
      buff = strdup(InputAnswer("Input file name to read tree from: "));
      buf1 = InputAnswer("Input file name to save code to: ");
      TRS;
      break;
    case 3:
      printf("BACKEND\n");
      buff = strdup(InputAnswer("Input file name to read tree from: "));
      buf1 = InputAnswer("Input file name to save asm code to: ");
      TRS;
      break;
    case 4:
      printf("MIDDLEEND\n");
      buff = strdup(InputAnswer("Input file name to read tree from: "));
      buf1 = InputAnswer("Input file name to save tree to: ");
      TRS;
      break;
    case 5:
      if (last == 0)
      {
        printf("Please, translate tree before dump\n");
        break;
      }
      printf("DUMP\n");
      buff = InputAnswer("Input file name to dump to: ");
      prg[last - 1]->dump_root(buf1);
      break;
    default:
      printf("Incorrect number of command, try again\n");
      break;
    }
  }
  #undef TPS
} /* End of 'MainProcLoop' function */