#ifndef __LIST_H_
#define __LIST_H_

#include <stdio.h>
#include <iostream>
#include <assert.h>
//#include "D:\\TX\TXLib.h"

#pragma warning (disable: 4996)

#define LIST_LOCATION __FILE__, __LINE__, __FUNCSIG__
#define var_name(var) #var


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

/**
 * \brief Clamping value function (template).
 * \param [in] value  Value to clamp.
 * \param [in]  min   Minimal value.
 * \param [in]  max   Maximum value.
 * \return Clamping value.
 */
template <typename Data>
Data Clamp( Data value, Data min, Data max )
{
  return value > max ? max : value < min ? min : value;
} /* End of 'Clamp' function */

// Template list class
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
    LST_POI_ERROR        =     6
  };
  
// Constants
  const size_t list_start_size = 7;
  const size_t list_delta = 3;
  const int LAST_FREE = -1;

  template <typename Data>
  const Data list_poison_value = -6699;

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

  template <typename Data>
  class List
  {
  private:
    list_elem<Data> *elems;
    int free_plc, head, tail;
    size_t size, maxsize;
    int error;

  public:

    /**
     * \brief Default list constructor function (template).
     * \param None.
     * \return None.
     */
    List( void ) : size(0),
                   free_plc(1),
                   maxsize(list_start_size),
                   error(0),
                   elems(nullptr),
                   head(1),
                   tail(0)

    {
      DataCalloc();
    } /* End of 'List' function */

    /**
     * \brief Push value to the end of list function (template).
     * \param [in]  value   Value to push.
     * \return true if all is ok.
     * \return false otherwise.
     */
    bool Push_tail( Data value )
    {
      return Insert_af(value, tail);
    } /* End of 'Push_tail' function */

    /**
     * \brief Push value to the begin of list function (template).
     * \param [in]  value   Value to push.
     * \return true if all is ok.
     * \return false otherwise.
     */
    bool Push_head( Data value )
    {
      return Insert_bef(value, head);
    } /* End of 'Push_head' function */

    /**
     * \brief Insert value to the list before element function (template).
     * \param [in]  value   Value to push.
     * \param [in]  num     logical num to push
     * \return true if all is ok.
     * \return false otherwise.
     */
    bool Insert_bef( Data value, size_t num )
    {
      if (elems[free].next == LAST_FREE)
      {
        // assert here
        return false;
      }
      size_t elem_num = Find(num);

      if (elem_num == head)
        head = elem_num;

      // Assign values in new element
      elems[free].data = value;
      elems[free].next = elem_num;
      elems[free].prev = elems[elem_num].prev;

      // Insert element in list
      if (elems[free].prev != 0)
        elems[elems[free].prev].next = free;
      elems[elem_num].prev = free;

      // New first free element
      free = elems[free].next;
      return true;
    } /* End of 'Insert_bef' function */

    /**
     * \brief Insert value to the list after element function (template).
     * \param [in]  value   Value to push.
     * \param [in]  num     logical num to push
     * \return true if all is ok.
     * \return false otherwise.
     */
    bool Insert_af( Data value, size_t num )
    {
      if (elems[free].next == LAST_FREE)
      {
        // assert here
        return false;
      }
      size_t elem_num = Find(num);

      if (elem_num == tail)
        tail = elem_num;

      // Assign values in new element
      elems[free].data = value;
      elems[free].prev = elem_num;
      elems[free].next = elems[elem_num].next;

      // Insert element in list
      if (elems[free].next != 0)
        elems[elems[free].next].prev = free;
      elems[elem_num].next = free;

      // New first free element
      free = elems[free].next;
      return true;
    } /* End of 'Insert_af' function */

    /**
     * \brief Pop value from list function (template).
     * \param [out]     err    Pointer to return a value.
     * \return true if all is OK.
     * \return false otherwise.
     */
    bool Pop( Data *value )
    {
      return true;
    } /* End of 'Pop' function */

    /**
     * \brief List destructor function (template).
     * \param None.
     * \return None.
     */
    ~List( void )
    {
      if (elems != nullptr)
        free(elems);
    } /* End of '~List' function */

    /**
     * \brief List dump function (template).
     * \param [in]  reason Dump reason.
     * \param [in]  filename Name of a file where Dump function called.
     * \param [in]  line Line where Dump function called.
     * \param [in]  funcname Name of a function where Dump function called.
     * \return None.
     */
    void Dump(const char reason[], const char filename[],
      int line, const char funcname[]) const
    {
    } /* End of 'Dump' function */

  private:
    /**
     * \brief Allocate memory for list function(template)
     * \param None
     * \return true if all is OK
     * \return false otherwise.
     */
    bool DataCalloc( void )
    {
      elems = (list_elem<Data> *)calloc(maxsize, sizeof(list_elem<Data>));
      if (elems == nullptr)
      {
        return false;
      }
      // Fill first value of data as poison
      elems[0].data = list_poison_value<Data>;

      // Fill next array
      FillIncr();

      #undef MEM_ALL
      return true;
    } /* End of 'DataCalloc' function */

    /**
     * \brief Fill array next with increasing values.
     * \param [in, out]  Pointer to array.
     */
    void FillIncr( void )
    {
      for (size_t i = 1; i < maxsize - 1; i++)
        elems[i].next = i + 1;
      elems[maxsize - 1].next = LAST_FREE;
    } /* End of ''FillIncr' function */

    /**
     * \brief Find element in list by logical number.
     * \param [in] num logical number
     * \return actual element number.
     * \return -1 if element was not find.
     */
    size_t Find( size_t num )
    {
      if (num >= size)
        return -1;
      size_t act = 0;
      if (size - num < num)
        act = GoTail(num);
      else
        act = GoHead(num);
      
      return act;
    } /* End of 'Find' function */

    /**
     * \brief Find element from tail by logical number.
     * \param [in] num logical number
     * \return actual element number.
     * \return -1 if element was not find.
     */
    size_t GoTail( size_t num )
    {
      if (num >= size)
        return -1;

      num = size - 1 - num;
      size_t count = 0, act = tail;

      while (count++ < num)
        act = elems[act].prev;
      
      return act;
    } /* End of 'GoTail' function */

    /**
     * \brief Find element from head by logical number.
     * \param [in] num logical number
     * \return actual element number.
     * \return -1 if element was not find.
     */
    size_t GoHead( size_t num )
    {
      if (num >= size)
        return -1;
      size_t count = 0, act = head;

      while (count++ < num)
        act = elems[act].next;
      
      return act;
    } /* End of 'GoHead' function */

    /**
     * \brief List verificator function (template).
     * \return true if all is ok.
     * \return false otherwise.
     */
    bool ListOk( void )
    {
      
      return true;
    } /* End of 'ListOk' function */

    /**
     * Processing errors in list fucntion (template).
     * \param [in]  error_code  Error code value.
     * \return None.
     */
    void List_Process_Error(const char filename[], int line, const char funcname[]) const
    {
#define LIST_DUMP_CASE(ERR_CODE, STR)  case ERR_CODE:                                      \
                                               Dump(STR, filename, line, funcname);   \
                                               break;

      if (this == nullptr)
      {
        Dump("", LIST_LOCATION);
        return;
      }
      switch (error)
      {
        

      default:
        printf("Unrecognized error code = %d\n", error);
        break;
      }

#undef LIST_DUMP_CASE
    } /* End of 'List_Process_Error' function */

    /**
     * \brief List resize function (template).
     * \param [in]  new_size New size value.
     * \return true if all is ok.
     * \return false otherwise.
     */
    bool Resize( size_t new_size )
    {
      return true;
    } /* End of 'Resize' function */


    /**
     * \brief List assertion function (template).
     * \return true if all id OK.
     * \return false otherwise.
     */
    bool Assert( const char filename[], int line, const char funcname[] )
    {
      if (error != 0)
      {
        List_Process_Error(filename, line, funcname);
        return false;
      }
      if (!ListOk())
      {
        List_Process_Error(filename, line, funcname);
        return false;
      }
      return true;
    } /* End of 'Assert' function */
  };
}

#endif /* __LIST_H_ */

