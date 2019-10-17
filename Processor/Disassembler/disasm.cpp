#include "disasm.h"

/**
 * \brief Disassembler class constructor.
 * \return None.
 */
ad6Disasm::ad6Disasm( void )
{
  buf_in = nullptr;
  buf_out = nullptr;
  buf_in_size = 0;
  buf_out_size = 0;
} /* End of 'ad6Disasm' function */

/**
 * \brief Program disassemble function.
 * \param [in] file_in   Name of a file to read from.
 * \param [in] file_out  Name of a file to write to.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6Disasm::Disassembly( const char file_in[], const char file_out[] )
{
  assert(file_in != nullptr);
  assert(file_out != nullptr);

  if (!FillPrgFromFile(file_in))
    return false;
  
  buf_out = (char *)calloc(buf_in_size * 100, sizeof(buf_out[0]));
  if (buf_out == nullptr)
    return false;

  char *bptr_o = buf_out;
  char *bptr_in = (char *)((int *)buf_in + 2);
  if (*((int *)buf_in) != AD6_SIGNATURE)
  {
    printf("Incorrect signature: %d\n", *((int *)buf_in));
    return false;
  }
  while (bptr_in - buf_in != buf_in_size)  
  {
    char *prev = nullptr;
#define DEF_CMD(name, num, len, code, syntax)   \
    case num:                                   \
      sprintf(bptr_o, #name " ");               \
      prev = bptr_o;                            \
      bptr_o += strlen(#name) + 1;              \
      bptr_in++;                                \
      code;                                     \
      *bptr_o++ = '\n';                         \
      buf_out_size += bptr_o - prev;            \
      break;

    switch(*bptr_in)
    {
#include "..\commands.h"
    default:
      printf("Incorrect number of command: %c\n", *bptr_in);
      return false;
    }
  }

  if (!PutBufToFile(file_out, buf_out, buf_out_size))
    return false;

#undef DEF_CMD
  return true;
} /* End of 'Disassembly' function */

/**
 * \brief Assembler get compiled code of program from file function.
 * \param file_name  Name of a file to read from.
 * \return true if all is ok.
 * \return false otherwise.
 */
bool ad6Disasm::FillPrgFromFile( const char file_name[] )
{
  int err_code = 0;
  
  buf_in = LoadAndCreateStrings(file_name, &buf_in_size, &err_code);
  if (buf_in == nullptr)
  {
    LACS_Process_Error(err_code);
    return false;
  }

  return true;
} /* End of 'FillPrgFromFile' function */

/**
 * \brief Disassembler class destructor.
 * \return None.
 */
ad6Disasm::~ad6Disasm( void )
{
  free(buf_out);
  free(buf_in);
} /* End of '~ad6Disasm' function */