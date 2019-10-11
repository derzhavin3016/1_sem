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
 * \brief Program assemble function.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6Asm::Assembly( void )
{
  char file_name[MAX_NAME] = {{0}};
  int IsOk = Input("# Stack calculator from the file program.\n\n"
                   "Input file name to start: ", "%s", file_name);
  COND_CHECK(!IsOk);
  COND_CHECK(!FillTxtFromFile(file_name));
  
  buf_out = (char *)calloc(buf_in_size, sizeof(buf_out[0]));
  COND_CHECK(buf_out == nullptr);
  char *bptr = buf_out + 2 * sizeof(int);

  *(int *)buf_out = signature;
  *((int *)buf_out + 1) = AD6_VER_NO;
  buf_out_size += 2 * sizeof(int);
  for (size_t PC = 0; PC < prog_size; PC++)
  {
    char promt[MAX_NAME] = {{0}};
    int pos = 0;
    if (sscanf(prog[PC].str, "%s %n", promt, &pos) != 1)
      return false;
    if (!StrCompareBegin(promt, "end"))
    {
      *bptr++ = AD6ASM_END;
      *bptr++ = '\n';
      buf_out_size += 2;
    }
    else if (!StrCompareBegin(promt, "push"))
    {
      int num_push = 0;
      if (sscanf(prog[PC].str + pos, " %d", &num_push) == 1)
      {
        *bptr++ = AD6ASM_PUSH_NUM;
        *((int *)bptr) = num_push * 1000;
        bptr = (char *)((int *)bptr + 1);
        *bptr++ = '\n';
        buf_out_size += 2 + sizeof(int);
      }
      else
      {
        printf("Error. No argument for push command in line %d\n", PC + 1);
        return false;
      }
    }
    else if (!StrCompareBegin(promt, "pop"))
    {
      *bptr++ = AD6ASM_POP;
      *bptr++ = '\n';
      buf_out_size += 2;
    }
    else if (!StrCompareBegin(promt, "add"))
    {
      *bptr++ = AD6ASM_ADD;
      *bptr++ = '\n';
      buf_out_size += 2;
    }
    else
    {
      printf("Unrecognized command: '%s'\n", promt);
      return false;
    }
  }

  IsOk = Input("Input file name to save code: ", "%s", file_name);
  COND_CHECK(!IsOk);
  COND_CHECK(!PutTxtToFile(file_name));

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
 * \brief Assembler put compiled program to file function.
 * \param file_name  Name of a file to write.
 * \return true if all is ok.
 * \return false otherwise.
 */
bool ad6Asm::PutTxtToFile( const char file_name[] )
{
  FILE *out = fopen(file_name, "wb");

  COND_CHECK(out == nullptr);


  fwrite(buf_out, sizeof(char), buf_out_size, out);

  fclose(out);
  return true;
} /* End of 'PutTxtToFile' function */

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

