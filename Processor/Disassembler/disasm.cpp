#include "disasm.h"

/**
 * \brief Disassembler class constructor.
 * \return None.
 */
ad6::Disasm::Disasm( void ) : code(nullptr),
                              buf_out(nullptr),
                              code_size(0),
                              buf_out_size(0)
{
} /* End of 'Disasm' function */

/**
 * \brief Program disassemble function.
 * \param [in] file_in   Name of a file to read from.
 * \param [in] file_out  Name of a file to write to.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::Disasm::Disassembly( const char file_in[], const char file_out[] )
{
  assert(file_in != nullptr);
  assert(file_out != nullptr);

  if (!FillPrgFromFile(file_in))
    return false;
  
  buf_out = (char *)calloc(code_size * 100, sizeof(buf_out[0]));
  if (buf_out == nullptr)
  {
    printf("Error with buffer memory allocation\n");
    return false;
  }

  char *bptr_o = buf_out;
  char *code_ptr = (char *)((int *)code + 2);
  if (*((int *)code) != SIGNATURE)
  {
    printf("Incorrect signature: %d\n", *((int *)code));
    return false;
  }

  char *prev = nullptr;

  #define DEF_CMD(name, num, len, code_pr, syntax_asm, makecode)   \
    case num:                                   \
      sprintf(bptr_o, #name " ");               \
      prev = bptr_o;                            \
      bptr_o += strlen(#name) + 1;              \
      code_ptr++;                                \
      code_pr;                                  \
      *bptr_o++ = '\n';                         \
      buf_out_size += bptr_o - prev;            \
      break;

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
bool ad6::Disasm::FillPrgFromFile( const char file_name[] )
{
  assert(file_name != nullptr);

  int err_code = 0;
  
  code = LoadAndCreateStrings(file_name, &code_size, &err_code);
  if (code == nullptr)
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
ad6::Disasm::~Disasm( void )
{
  if (buf_out != nullptr)
    free(buf_out);
  if (code != nullptr)
    free(code);
  code = buf_out = nullptr;
} /* End of '~Disasm' function */