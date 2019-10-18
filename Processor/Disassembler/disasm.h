#ifndef __DISASM_H_
#define __DISASM_H_

#include "../proc.h"

#define COND_CHECK(COND)    if (COND)        \
                              return false


const int MAX_NAME = 100;

class ad6Disasm
{
private:
  char *buf_in, *buf_out;
  size_t buf_in_size, buf_out_size;
public:

  ad6Disasm( void );
  
  bool Disassembly( const char file_in[], const char file_out[] );

  ~ad6Disasm( void );
private:

  bool FillPrgFromFile( const char file_name[] );
};


#endif /* __DISASM_H_ */