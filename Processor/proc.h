#ifndef __PROC_H_
#define __PROC_H_

#include <math.h>
#include "Stack/stack.h"
#include "Strings/strings.h"
#include "Input/input.h"
//#include "Graphics/graph.h"

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
  const size_t FRAME_Y = 200;
  const size_t FRAME_X = 200;

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
 
  /**
   * \brief clamp value function.
   * \param value VAlue to clamp.
   * \param min  bottom clamp value.
   * \param max  upper clamp value.
   * \return clamped value
   */
  template <typename Data>
  __inline Data Clamp( Data value, Data min, Data max )
  {
    return value > max ? max : value < min ? min : value;
  } /* End of 'Clamp' function */

  enum Asm_CMD
  {
  #include"commands.h"
  };

  class Render;
}

#undef DEF_CMD

#endif /* __PROC_H_ */
