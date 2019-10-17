#ifndef __DISASM_H_
#define __DISASM_H_

#include "../proc.h"

#define COND_CHECK(COND)    if (COND)        \
                              return false


const int MAX_NAME = 100;

class ad6Disasm
{
private:
  //String *prog;
  char *buf_in, *buf_out;
  size_t /*prog_size,*/ buf_in_size, buf_out_size;
public:

  /**
   * \brief Disassembler class constructor.
   * \return None.
   */
  ad6Disasm( void );
  
  /**
   * \brief Program disassemble function.
   * \param [in] file_in   Name of a file to read from.
   * \param [in] file_out  Name of a file to write to.
   * \return true if all is OK.
   * \return false otherwise.
   */
  bool Disassembly( const char file_in[], const char file_out[] );

  /**
   * \brief Disassembler class destructor.
   * \return None.
   */
  ~ad6Disasm( void );
private:

  /**
   * \brief Assembler get compiled code of program from file function.
   * \param file_name  Name of a file to read from.
   * \return true if all is ok.
   * \return false otherwise.
   */
  bool FillPrgFromFile( const char file_name[] );
};


#endif /* __DISASM_H_ */