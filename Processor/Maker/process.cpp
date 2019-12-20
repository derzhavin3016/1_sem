#include "process.h"

/**
 * \brief Program process loop function
 */
void ProcessLoop( void )
{
  Asm as;
  Disasm dis;
  Proc cp;
  char *buff = nullptr, *buf1 = nullptr;
  int val = 0;
  
  printf(GREETING);

  while (1)
  {
    printf("Input number to start: \n");
    scanf("%d", &val);
    switch (val)
    {
    case 0:
      printf(PARTING);
      return;
    case 1:
      printf("ASSEMBLER\n");
      buff = strdup(InputAnswer("Input filename to get asm code: \n"));
      buf1 = InputAnswer("Input filename to save binary code to (only name): \n");
      buf1 = strcat(buf1, ".adasm");
      
      CPU_ASSERT(as.Assembly(buff, buf1), "Assembler error");

      free(buff);
      break;
    case 2:
      printf("DISASSEMBLER\n");
      buff = strdup(InputAnswer("Input filename to get binary code: \n"));
      buf1 = InputAnswer("Input filename to save asm code to (only name): \n");

      CPU_ASSERT(dis.Disassembly(buff, buf1), "Disassembler error");

      free(buff);
      break;
    case 3:
      printf("EXECUTOR\n");
      buff = InputAnswer("Input filename to get binary code: \n");

      CPU_ASSERT(cp.Execute(buff), "Error with executer");

      break;
    default:
      printf("So strange command number %d, please, try again\n", val);
      break;
    }
  }
} /* End of 'Process' function */

/**
 * \brief Get user's answer from stdout function.
 * \brief This function put scanned string to static buffer, allocate memory for the answer and return pointer.
 * \param printfstr   String for printf.
 * \param  ...        Printf parameters.
 * \return  pointer to the answer.
 * \warning Using static buffer.
 */
char * InputAnswer( const char printfstr[], ... )
{
  static char buf[MAX_NAME] = {};
  va_list args;

  if (printfstr == nullptr)
    return buf;

  va_start(args, printfstr);
  vprintf(printfstr, args);
  va_end(args);

  scanf("\n%1024[^\n]", buf);

  return buf;
} /* End of 'InputAnswer' function */