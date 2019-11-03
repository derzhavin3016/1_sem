#ifndef __LIST_DEF_H_
#define __LIST_DEF_H_

#pragma warning (disable: 4996)

#define LIST_LOCATION __FILE__, __LINE__, __FUNCSIG__
#define var_name(var) #var


#define LST_START_PROMT                          \
  "****************************************\n"   \
  "****************************************\n"   \
  "      Working with list program         \n"   \
  "***********   MENU   *******************\n"   \
  "*** 0. Exit                             \n"   \
  "*** 1. Push value to head               \n"   \
  "*** 2. Push value to tail               \n"   \
  "*** 3. Insert value before              \n"   \
  "*** 4. Insert value after               \n"   \
  "*** 5. Delete value                     \n"   \
  "*** 6. Dump                             \n"   \
  "*** 7. KILL                             \n"   \
  "****************************************\n"   \

#define LIST_COND_CHECK(COND, ERR_CODE, ret) if (COND)                          \
                                             {                                  \
                                              error = ERR_CODE;                \
                                               return ret;                      \
                                             }

#ifndef NDEBUG
  #define LST_ASSERT() if (!Assert(LIST_LOCATION)) \
                        throw error;
#else       
  #define LST_ASSERT() 
#endif

#define LIST_IF_FREE                                     \
  assert(elems[num].prev != LAST_FREE)

#define LIST_IF_LAST_FREE if (elems[free_plc].next == LAST_FREE)  \
                            Resize((maxsize - 1) * 2 + 1);        \

#define LIST_FST_PUSH if (size == 0)                       \
                      {                                    \
                        LIST_IF_LAST_FREE;                 \
                        int new_num = free_plc;            \
                        free_plc = elems[free_plc].next;   \
                        elems[new_num].data = value;       \
                        elems[new_num].prev = 0;           \
                        elems[new_num].next = 0;           \
                        head = tail = 1;                   \
                        size++;                            \
                        return head;                       \
                      }

#define LIST_INSERT(REL_ELEM, REL_ANOTHER, DIR_END)                  \
  if (num == DIR_END)                                                \
    DIR_END = free_plc;                                              \
   int new_num = free_plc;                                           \
                                                                     \
  /* New first free element */                                       \
  free_plc = elems[free_plc].next;                                   \
  size++;                                                            \
                                                                     \
  /* Assign values in new element */                                 \
  elems[new_num].data = value;                                       \
  elems[new_num].##REL_ELEM = num;                                   \
  elems[new_num].##REL_ANOTHER = elems[num].##REL_ANOTHER;           \
                                                                     \
  /* Insert element in list */                                       \
  if (elems[new_num].##REL_ANOTHER != 0)                             \
    elems[elems[new_num].##REL_ANOTHER].##REL_ELEM = new_num;        \
  elems[num].##REL_ANOTHER = new_num;

#define LIST_IF_COR_NUM                                              \
  assert(num > 0 && num < maxsize)

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
  const Data LIST_POISON_VALUE = -6699;

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

