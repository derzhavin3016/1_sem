#include "processor.h"

/**
 * \brief Processor class default constructor.
 */
ad6Proc::ad6Proc( void )
{
  code = nullptr;
  code_size = 0;
} /* End of 'ad6Proc' function */

/**
 * \brief Processor class default constructor.
 * \param [in] file_in  Name of a file with binary code.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6Proc::Execute( const char file_in[] )
{
  assert(file_in != nullptr);

  if (!FillBuf(file_in))
    return false;

  char *code_ptr = (char *)((int *)code + 2);
  if (*((int *)code) != AD6_SIGNATURE)
  {
    printf("Incorrect signature: %d\n", *((int *)code));
    return false;
  }
  while (code_ptr - code != code_size)  
  {
#define DEF_CMD(name, num, len, code_pr, syntax_asm_asm, makecode)   \
    case num:                                   \
      code_ptr++;                               \
      makecode;                                 \
      break;                                    \

    switch(*code_ptr)
    {
#include "..\commands.h"
    default:
      printf("Incorrect number of command: %c\n", *code_ptr);
      return false;
    }
  }

#undef DEF_CMD

  return true;
} /* End of 'Execute' function */


/**
 * \brief Fill buffer with bynary code from file function.
 * \param [in] file_in  Name of a file with binary code.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6Proc::FillBuf( const char file_in[] )
{
  assert(file_in != nullptr);

  int err = 0;
  code = LoadAndCreateStrings(file_in, &code_size, &err);
  if (code == nullptr)
  {
    LACS_Process_Error(err);
    return false;
  }

  return true;
} /* End of 'FillBuf' function */

/**
 * \brief Processor class default destructor.
 */
ad6Proc::~ad6Proc( void )
{

} /* End of '~ad6Proc' function */