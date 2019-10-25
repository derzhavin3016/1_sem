#ifndef __PROC_H_
#define __PROC_H_

#include <math.h>
#include "Stack/stack.h"
#include "Strings/strings.h"
#include "Input/input.h"

#pragma warning (disable: 4244)

const int ACCURACY = 1000;

#define COND_CHECK(COND)    if (!(COND))        \
                              return false

#define DEF_CMD(name, num, len, code_pr, syntax_asm, makecode) \
  ASM_CMD_##name = num,

#define CHECK_FILL_BUF COND_CHECK((code = FillBuf(file_in, &code_size)) != nullptr)


const int MAX_NAME = 100;

namespace ad6
{
  class Disasm;

  class Asm;

  class Proc;

  const int SIGNATURE = 'A6DA';

  static int VER_NO = 0;

  struct File_Header
  {
    int sign;
    int version;
  };
 
  
  enum Asm_CMD
  {
  #include"commands.h"
  };


}

#undef DEF_CMD

#endif /* __PROC_H_ */
