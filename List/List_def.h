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
  "****************************************\n"   \

#define LIST_COND_CHECK(COND, ERR_CODE) if (COND)                          \
                                         {                                  \
                                           error = ERR_CODE;                \
                                           return false;                    \
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
                          {                                       \
                            /*assert here*/                       \
                            return -1;                            \
                          }

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
    elems[elems[new_num].##REL_ANOTHER].##REL_ELEM = free_plc;       \
  elems[num].##REL_ANOTHER = new_num;

#define LIST_IF_COR_NUM                                              \
  assert(num > 0 && num < maxsize)

#endif /* __LIST_DEF_H_ */

