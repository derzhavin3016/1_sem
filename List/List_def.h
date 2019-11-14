#ifndef __LIST_DEF_H_
#define __LIST_DEF_H_

#pragma warning (disable: 4996)

namespace ad6
{
  /**
   * \brief Find max of two values function(template).
   * \param [in] a, b values
   * \return maximum of the values.
   */
  template <typename Data>
  __inline Data Max( Data a, Data b )
  {
    return a > b ? a : b;
  } /* End of 'Max' function */

  enum LST_ERR
  {
    LST_UNDERFLOW        =    -1,
    LST_OVERFLOW         =     1,
    LST_SIZE_ERROR       =     2,
    LST_DATA_ERROR       =     3,
    LST_MEM_ERROR        =     4,
    LST_NEW_SIZE_ERROR   =     5,
    LST_POI_ERROR        =     6,
    LST_NO_FREE_ERROR    =     7,
    LST_HEAD_ERROR       =     8,
    LST_TAIL_ERROR       =     9
  };
  
  // Constants
  const size_t LIST_START_SIZE = 7;
  const size_t LIST_DELTA = 3;
  const int LAST_FREE = -2;
  const int FREE_PREV = -1;

  template <typename Data>
  const Data LIST_POISON_VALUE = nullptr;

  template <typename Data>
  struct list_elem
  {
  public:
    Data data;
    int next;
    int prev;

    // Default constructor
    list_elem( void ) : data(0),
                        next(0),
                        prev(0)
    {
    }
  };
}

#endif /* __LIST_DEF_H_ */

