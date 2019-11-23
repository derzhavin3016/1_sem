#ifndef __DEFS_H_
#define __DEFS_H_

#include <stdlib.h>
#include <iostream>
#include "D:\TX\TXLib.h"
#include "List\List.h"
#include "Stack\stack.h"
#include "Differentiator/STOCK.H"
#include "Processor/Strings/strings.h"
#include "Strs/strs.h"
#include <assert.h>


namespace ad6
{
  // Process error structure
  struct Error
  {
    const char* descr;
    int line;
    const char* file;
    const char* func;
    //Error* reason;

    Error( const char* des, int l, const char* filename, const char* function ) : descr(des),
                                                                                  line(l),
                                                                                  file(filename),
                                                                                  func(function)
    {
    }
  };

  /**
   * \brief Swap two values function by pointers (template).
   * \param [out]   *a  Pointer to first value.
   * \param [out]   *b  Pointer to second value.
   * \return None.
   */
  template <typename Data>
  void Swap( Data* A, Data *B )
  {
    Data tmp = *A;
    *A = *B;
    *B = tmp;
  } /* End of 'Swap' function */


}

#endif /* __DEFS_H_ */

