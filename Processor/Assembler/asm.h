#ifndef __asm_h_

#include "proc.h"

#define COND_CHECK(COND)    if (COND)        \
                              return false


const int MAX_NAME = 100;

class ad6Asm
{
private:
  String *prog;
  const int signature = 'AD6A';
  char *buf_in, *buf_out;
  size_t prog_size, buf_in_size, buf_out_size;
public:

  /**
   * \brief Assembler class constructor.
   * \return None.
   */
  ad6Asm( void );
  
  /**
   * \brief Program assemble function.
   * \return true if all is OK.
   * \return false otherwise.
   */
  bool Assembly( void );

  /**
   * \brief Assembler class destructor.
   * \return None.
   */
  ~ad6Asm( void );
private:

  /**
   * \brief Assembler put compiled program to file function.
   * \param file_name  Name of a file to write.
   * \return true if all is ok.
   * \return false otherwise.
   */
  bool PutTxtToFile( const char file_name[] );

  /**
   * \brief Assembler get text of program from file function.
   * \param file_name  Name of a file to read from.
   * \return true if all is ok.
   * \return false otherwise.
   */
  bool FillTxtFromFile( const char file_name[] );
};

#endif /* __asm_h_ */

