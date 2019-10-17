#ifndef __PROC_H_
#define __PROC_H_

#include <math.h>
#include "Stack/stack.h"
#include "Strings/strings.h"
#include "Input/input.h"

static int AD6_VER_NO = 0;

const int AD6_SIGNATURE = 'A6DA';

#define DEF_CMD(name, num, len, code, syntax) \
  ASM_CMD_##name = num,

enum AD6ASM_CMD
{
#include"commands.h"
};

#undef DEF_CMD

#endif /* __PROC_H_ */
