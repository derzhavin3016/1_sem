#include "processor.h"

/**
 * \brief Processor class default constructor.
 */
ad6::Proc::Proc( void ) : code(nullptr), code_size(0)
{
  for (size_t i = 0; i < REGS_SIZE; i++)
    regs[i] = 0;
} /* End of 'Proc' function */

/**
 * \brief Processor class default constructor.
 * \param [in] file_in  Name of a file with binary code.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::Proc::Execute( const char file_in[] )
{
  assert(file_in != nullptr);

  CHECK_FILL_BUF;

  char *code_ptr = (char *)((int *)code + 2);
  if (*((int *)code) != SIGNATURE)
  {
    printf("Incorrect signature: %d\n", *((int *)code));
    return false;
  }

  Stack<int> stk, func_stk;

  #define DEF_CMD(name, num, len, code_pr, syntax_asm_asm, makecode)   \
    case num:                                   \
      code_ptr++;                               \
      makecode;                                 \
      break;                                    \

  int jmp_counter = 0;
  while (code_ptr - code != code_size)  
  {
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
 * \brief Processor class default destructor.
 */
ad6::Proc::~Proc( void )
{
  if (code != nullptr)
    free(code);
} /* End of '~Proc' function */

/**
 * \brief Input coeffc to registers function.
 * \param prc pointer to processor class.
 * \param a, b, c, coefficients.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::Proc::InputCoeffs( double a, double b, double c )
{
  regs[0] = a * ACCURACY;
  regs[1] = b * ACCURACY;
  regs[2] = c * ACCURACY;

  return true;
} /* End of 'InputCoeffs' function */