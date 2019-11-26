#ifndef __DEFS_H_
#define __DEFS_H_

#include <stdlib.h>
#include <iostream>
//#include "D:\TX\TXLib.h"
#include "List\List.h"
#include "Stack\stack.h"
#include "Differentiator/STOCK.H"
#include "Processor/Strings/strings.h"
#include "Strs/strs.h"
#include <assert.h>

/* Debug memory allocation support */ 
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h> 
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy 
{ 
public: 
  /* Class constructor */
  __Dummy( void ) 
  { 
    SetDbgMemHooks(); 
  } /* End of '__Dummy' constructor */
} __ooppss;

#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */


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

