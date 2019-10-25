#ifndef __DISASM_H_
#define __DISASM_H_

#include "../proc.h"


class ad6::Disasm
{
private:
  char *code, *buf_out;
  size_t code_size, buf_out_size;
public:

  Disasm( void );
  
  bool Disassembly( const char file_in[], const char file_out[] );

  ~Disasm( void );
private:

  bool FillPrgFromFile( const char file_name[] );
};


#endif /* __DISASM_H_ */