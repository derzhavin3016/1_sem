#ifndef __PROCESSOR_H_
#define __PROCESSOR_H_

#include "../proc.h"

class ad6Proc
{
public:
  Stack<int> stk;
  char *code;
  size_t code_size;
public:

  // Default constructor
  ad6Proc( void );

  bool Execute( const char file_in[] );

  // Destructor
  ~ad6Proc( void );

private:

  bool FillBuf( const char file_in[] );
};

#endif /* __PROCESSOR_H_ */
