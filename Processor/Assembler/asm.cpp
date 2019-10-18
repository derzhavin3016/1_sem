#include "asm.h"

/**
 * \brief Assembler class constructor.
 * \return None.
 */
ad6Asm::ad6Asm( void )
{
  prog_size = 0;
  prog = nullptr;
  buf_in = nullptr;
  buf_out = nullptr;
  buf_in_size = 0;
  buf_out_size = 0;
} /* End of Assembler constructor */

/**
 * \brief Program Assembly function.
 * \param [in] file_in   Name of a file to read from.
 * \param [in] file_out  Name of a file to write to.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6Asm::Assembly( const char file_in[], const char file_out[] )
{
  COND_CHECK(!FillTxtFromFile(file_in));
  
  buf_out = (char *)calloc(buf_in_size, sizeof(buf_out[0]));
  COND_CHECK(buf_out == nullptr);
  char *bptr = buf_out + 2 * sizeof(int);

  *(int *)buf_out = AD6_SIGNATURE;
  *((int *)buf_out + 1) = AD6_VER_NO;
  buf_out_size += 2 * sizeof(int);
  for (size_t PC = 0; PC < prog_size; PC++)
  {
    char promt[MAX_NAME] = {{0}};
    int pos = 0;  
    if (sscanf(prog[PC].str, "%s %n", promt, &pos) != 1)
      return false;
    if (0);
#define DEF_CMD(name, num, len, code_pr, syntax_asm, makecode) \
    else if (StrCompareBegin(promt, #name) == 0)    \
    {                                         \
      *bptr++ = num;                          \
      buf_out_size++;                         \
      syntax_asm                                  \
    } 
    
#include "../commands.h"

    else
    {
      printf("Unrecognized command: '%s'\n", promt);
      return false;
    }
  }
  COND_CHECK(!PutBufToFile(file_out, buf_out, buf_out_size));

#undef DEF_CMD
  return true;
} /* End of 'Assembly' function */

/**
 * \brief Assembler class destructor.
 * \return None.
 */
ad6Asm::~ad6Asm( void )
{
  free(prog);
  free(buf_in);
  free(buf_out);
} /* End of Assembler destructor */

/**
 * \brief Assembler get text of program from file function.
 * \param file_name  Name of a file to read from.
 * \return true if all is ok.
 * \return false otherwise.
 */
bool ad6Asm::FillTxtFromFile( const char file_name[] )
{
  int err_code = 0;
  
  buf_in = LoadAndCreateStrings(file_name, &buf_in_size, &err_code);
  if (buf_in == nullptr)
  {
    LACS_Process_Error(err_code);
    return false;
  }
  
  prog = CreateStringsPtrs(buf_in, buf_in_size, &prog_size, '\n');
  COND_CHECK(prog == nullptr);

  DelCom(prog, prog_size, '\n');

  return true;
} /* End of 'FillTxtFromFile' function */

