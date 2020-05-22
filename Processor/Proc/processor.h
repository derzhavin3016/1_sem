#ifndef __PROCESSOR_H_
#define __PROCESSOR_H_

#include "../proc.h"

namespace ad6
{
  const size_t REGS_SIZE = 4;
  const int MAX_JUMPS = 100;
  const int FUNC_REC = 100;
  const size_t RAM_SIZE = 1000;
}

class ad6::Proc
{
private:
  char *code;
  size_t code_size;
  int regs[REGS_SIZE];
  int RAM[RAM_SIZE];
public:

  // Default constructor
  Proc( void );

  bool Execute( const char file_in[] );

  bool InputCoeffs( double a, double b, double c );

  // Destructor
  ~Proc( void );
};


#endif /* __PROCESSOR_H_ */
