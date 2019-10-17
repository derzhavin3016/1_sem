#include "input.h"

/**
 * \brief Clear input buffer fucntion.
 * \return  NONE.
 */
void ClearBuffer( void )
{
  char sym = getchar();

  while (sym != '\n')
    sym = getchar();
} /* End of 'ClearBuffer' function */