#include "asm.h"

/**
 * \brief Assembler class constructor.
 * \return None.
 */
ad6::Asm::Asm( void ) : prog_size(0), 
                        prog(nullptr), 
                        code(nullptr), 
                        buf_out(nullptr),
                        code_size(0),
                        buf_out_size(0),
                        marks{0},
                        marks_size(0),
                        IsAsm(false)
{
} /* End of Assembler constructor */

/**
 * \brief Assembler parser function.
 * \param [in] file_in   Name of a file to read from.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::Asm::AsmParser( const char file_in[] )
{
  assert(file_in != nullptr);
  COND_CHECK(TxtGet(file_in));

  buf_out = (char *)calloc(code_size, sizeof(buf_out[0]));
  COND_CHECK(buf_out != nullptr);
  char *bptr = buf_out + sizeof(File_Header);

  File_Header *Header = (File_Header *)buf_out;

  Header->sign = SIGNATURE;
  Header->version = VER_NO;
  buf_out_size += sizeof(File_Header);
  const char *mk = nullptr;

  for (size_t PC = 0; PC < prog_size; PC++)
  {
    char promt[MAX_NAME] = {{0}};
    int pos = 0;  
    if (sscanf(prog[PC].str, "%s %n", promt, &pos) != 1)
      return false;
  
  #define DEF_CMD(name, num, len, code_pr, syntax_asm, makecode) \
    else if (StrCompareBegin(promt, #name) == 0)    \
    {                                               \
      *bptr++ = num;                                \
      buf_out_size++;                               \
      syntax_asm                                    \
    } 
   
    else if ((mk = (char *)memchr(prog[PC].str, ':', prog[PC].len)) != nullptr)
    {
      if (!IsAsm)
      {
        /*if (FindMark(prog[PC].str))
        {
          printf("Same mark in line %d: '%s'\n", PC + 1, promt);
        }*/
        marks[marks_size].mrk = prog[PC].str;
        marks[marks_size++].line = static_cast<int>(buf_out_size);
      }
    }

   #include "../commands.h"

    else
    {
      printf("Unrecognized command in line %zd: '%s'\n", PC + 1, promt);
      return false;
    }
  }
#undef DEF_CMD
  IsAsm = true;
  return true;
} /* End of 'AsmParser' function */

/**
 * \brief Program Assembly function.
 * \param [in] file_in   Name of a file to read from.
 * \param [in] file_out  Name of a file to write to.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::Asm::Assembly( const char file_in[], const char file_out[] )
{
  assert(file_in != nullptr);
  assert(file_out != nullptr);

  if (!AsmParser(file_in))
    return false;
  if (buf_out != nullptr)
  {
    free(buf_out);
    buf_out_size = 0;
  }
  if (prog != nullptr)
  {
    free(prog);
    prog_size = 0;
  }
  //if (code != nullptr)
  //{
  //  free(code);
  //  code_size = 0;
  //}
  if (!AsmParser(file_in))
    return false;

  COND_CHECK(PutBufToFile(file_out, buf_out, buf_out_size));
  return true;
} /* End of 'Assembly' function */

/**
 * \brief Assembler class destructor.
 * \return None.
 */
ad6::Asm::~Asm( void )
{
  if (prog != nullptr)
    free(prog);
  if (code != nullptr)
    free(code);
  if (buf_out != nullptr)
    free(buf_out);
} /* End of Assembler destructor */

/**
 * \brief Assembler get text of program from file function.
 * \param file_name  Name of a file to read from.
 * \return true if all is ok.
 * \return false otherwise.
 */
bool ad6::Asm::TxtGet( const char file_in[] )
{
  assert(file_in != nullptr);

  CHECK_FILL_BUF;

  prog = CreateStringsPtrs(code, code_size, &prog_size, '\n');
  COND_CHECK(prog != nullptr);

  DelCom(prog, prog_size, '\n');

  return true;
} /* End of 'FillTxtFromFile' function */


/**
 * \brief Find mark in marks array function function.
 * \param str  String to find.
 * \return num of mark if string was find.
 * \return 0 otherwise.
 */
int ad6::Asm::FindMark( const char str[] )
{
  for (size_t i = 0; i < marks_size; i++)
    if (!StrCompareBegin(str, marks[i].mrk, ':'))
      return marks[i].line;

  return -2;
} /* End of 'FindMark' function */


