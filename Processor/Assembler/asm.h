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

  /**
   * \brief Assembler class constructor.
   * \return None.
   */
  ad6Asm( void );
  
  /**
   * \brief Program Assembly function.
   * \param [in] file_in   Name of a file to read from.
   * \param [in] file_out  Name of a file to write to.
   * \return true if all is OK.
   * \return false otherwise.
   */
  bool Assembly( const char file_in[], const char file_out[] );

  /**
   * \brief Assembler class destructor.
   * \return None.
   */
  ~ad6Asm( void );
private:

  /**
   * \brief Assembler get text of program from file function.
   * \param file_name  Name of a file to read from.
   * \return true if all is ok.
   * \return false otherwise.
   */
  bool FillTxtFromFile( const char file_name[] );
};

#endif /* __ASM_H_ */

