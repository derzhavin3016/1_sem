#ifndef __ASM_H_
#define __ASM_H_

#include "../proc.h"

const int MAX_MARKS = 1024;

struct Mark
{
  const char *mrk;
  int line;
};


class ad6::Asm
{
private:
  String *prog;
  Mark marks[MAX_MARKS];
  char *code, *buf_out;
  bool IsAsm;
  size_t prog_size, code_size, buf_out_size, marks_size;
public:
  
  Asm( void );
  
  bool Assembly( const char file_in[], const char file_out[] );

  ~Asm( void );

private:

  bool TxtGet( const char file_in[] );

  bool AsmParser( const char file_in[] );

  int FindMark( const char str[] );

  template <typename Data>
  bool GetArgs( char *buf_ptr, Data* values, size_t amount )
  {
    
  }
};

#endif /* __ASM_H_ */
