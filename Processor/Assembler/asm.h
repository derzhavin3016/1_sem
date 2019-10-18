#ifndef __ASM_H_
#define __ASM_H_

#include "../proc.h"

#define COND_CHECK(COND)    if (COND)        \
                              return false


const int MAX_NAME = 100;

class ad6Asm
{
private:
  String *prog;
  char *buf_in, *buf_out;
  size_t prog_size, buf_in_size, buf_out_size;
public:
  
  ad6Asm( void );
  
  bool Assembly( const char file_in[], const char file_out[] );

  ~ad6Asm( void );
private:

  bool FillTxtFromFile( const char file_name[] );
};

#endif /* __ASM_H_ */

