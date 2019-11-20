#ifndef __STACK_H_
#define __STACK_H_

#include <stdio.h>
#include <iostream>
#include <assert.h>

#pragma warning (disable: 4996)

#define STACK_LOCATION __FILE__, __LINE__, __FUNCSIG__
#define var_name(var) #var

#ifndef NDEBUG
  #define STK_ASSERT() if (!Assert(STACK_LOCATION)) \
                         throw error;
#else
  #define STK_ASSERT() 
#endif

#define STK_DATA(i) ((Data *)(data + sizeof(canary_t)))[i]

typedef unsigned long long canary_t;
typedef unsigned long long hash_t;

enum STK_ERR
{
  STK_UNDERFLOW        =    -1,
  STK_OVERFLOW         =     1,
  STK_OWL1_ERROR       =     2,
  STK_OWL2_ERROR       =     3,
  STK_HASH_ERROR       =     4,
  STK_SIZE_ERROR       =     5,
  STK_DATA_ERROR       =     6,
  STK_MEM_ERROR        =     7,
  STK_NEW_SIZE_ERROR   =     8,
  STK_POI_ERROR        =     9,
  STK_OWLDATA1_ERROR   =    10,
  STK_OWLDATA2_ERROR   =    11,
};

// Constants
  const canary_t owl1_control = 0xDEADBEEF;
  const canary_t owl2_control = 0xBEDAFADE;
  const canary_t owldata1_control = 0xBEDADEAD;
  const canary_t owldata2_control = 0xCCAAFFBB;
  const size_t stack_start_size = 7;
  const size_t stack_delta = 3;

  template <typename Data>
  const Data stack_poison_value = -6699;

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
 * \brief Hash calculation function (template).
 * \param [in, out] this_  Pointer to value to calculate hash.
 * \param [in]      size   Size of array (1 default).
 * \return Calculated hash.
 */
template <typename Data>
hash_t HashCalc( const Data *data, size_t size = 1 )
{
  assert(data != nullptr);
  const unsigned char *bytes = (unsigned char *)data;
  size_t bytes_len = sizeof(*data) * size;
  hash_t hash = 0;

  for (size_t i = 0; i < bytes_len; i++)
    hash += (i + 1) * (bytes[i] << (i % 4));

  return hash;
} /* End of 'HashCalc' function */

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

// Template stack class
template <typename Data>
class Stack
{
private:
  canary_t owl1;
  char *data;
  canary_t *owldata1, *owldata2;
  int size, maxsize;
  int error;
  hash_t hash;
  canary_t owl2;

#ifndef NDEBUG
  /**
   * \brief Stack hash calculation function (template).
   * \param None.
   * \return Calculated hash.
   * \warning This function also write new hash value to stack structure.
   */
  hash_t HashReCalc( void )
  {
    hash = 0;
    hash = HashCalc(this) +
      HashCalc(data, maxsize * sizeof(Data) + 2 * sizeof(canary_t));

    return hash;
  } /* End of 'HashReCalc' function */
#else
  #define HashReCalc() true
#endif

public:


  /**
   * \brief Return size of stack function
   */
  size_t Size( void )
  {
    return size;
  } /* End of 'Size' function */

  /**
   * \brief Default stack constructor function (template).
   * \param None.
   * \return None.
   */
  Stack( void )
  {
    assert(this != nullptr);


    owl1 = owl1_control;
    size = 0;
    maxsize = stack_start_size;
    error = 0;
    hash = 0;
    owl2 = owl2_control;
    if (!DataCalloc())
    {
      STK_ASSERT();
    }
    HashReCalc();

    STK_ASSERT();
  } /* End of 'Stack' function */

  /**
   * \brief Push value to stack function (template).
   * \param [in]      value Value to push.
   * \return true if all is ok.
   * \return false otherwise.
   */
  bool Push( Data value )
  {
    STK_ASSERT();

    if (size >= maxsize)
      if (!Resize(maxsize * 2))
      {
        STK_ASSERT();
        return false;
      }

    STK_DATA(size++) = value;
    HashReCalc();

    STK_ASSERT();
    return true;
  } /* End of 'Push' function */

  /**
   * \brief Pop value from stack function (template).
   * \param [out]     err    Pointer to return a value.
   * \return true if all is OK.
   * \return false otherwise.
   */
  bool Pop( Data *value )
  {
    STK_ASSERT();

    if (size == 0)
    {
      error = STK_UNDERFLOW;
      STK_ASSERT();
      return false;
    }
    if (maxsize > stack_start_size &&
      size == maxsize / 2 - stack_delta)
      if (!Resize(maxsize / 2))
      {
        STK_ASSERT();
        return false;
      }

    *value = STK_DATA(--size);
    FillPoi(size);
    HashReCalc();
    STK_ASSERT();
    return true;
  } /* End of 'Pop' function */

  /**
   * \brief Pop value from stack function (reload template).
   * \param None.
   * \return Data from stack.
   */
  Data Pop( void )
  {
    STK_ASSERT();

    if (size == 0)
    {
      STK_ASSERT();
      return 0;
    }
    if (maxsize > stack_start_size &&
      size == maxsize / 2 - stack_delta)
      if (!Resize(maxsize / 2))
      {
        STK_ASSERT();
        return 0;
      }

    Data value = STK_DATA(--size);
    FillPoi(size);
    HashReCalc();
    STK_ASSERT();
    return value;
  } /* End of 'Pop' function */

  /**
   * \brief Stack destructor function (template).
   * \param None.
   * \return None.
   */
  ~Stack( void )
  {
    owldata1 = owldata2 = nullptr;
    if (data != nullptr)
      free(data);
    size = 0;
  } /* End of '~Stack' function */

  /**
   * \brief Stack dump function (template).
   * \param [in]  reason Dump reason.
   * \param [in]  filename Name of a file where Dump function called.
   * \param [in]  line Line where Dump function called.
   * \param [in]  funcname Name of a function where Dump function called.
   * \return None.
   */
  void Dump( const char reason[], const char filename[],
    int line, const char funcname[] ) const
  {
    if (this == nullptr)
    {
      printf("Dump (%s) from %s (%d) %s\n", "Pointer to stack was nullptr",
        filename, line, funcname);
      printf("Stack \"%s\" [0x%p] (%s)\n{\n}", var_name(this), this, "ERROR!!!");
      return;
    }
    printf("Dump (%s) from %s (%d) %s\n", reason, filename, line, funcname);
    //printf("Stack \"%s\"[0x%llX] (%s)\n", var_name(*this), this, StackOk() ? "ok" : "ERROR!!!");

    printf("{\n");
    printf("  owl1 = %llX\n", owl1);
    printf("  size = %d\n", size);
    printf("  maxsize = %d\n", maxsize);
    printf("  data[%d]%s\n", maxsize, data == nullptr ?
      " - NULLPTR!!!" : "");
    printf("  {\n");
    if (data != nullptr)
    {
      printf("  owldata1 = %llX\n", *owldata1);
      for (int i = 0; i < maxsize; i++)
      {
        printf("  %c [%d] = ", i < size ? '*' : ' ', i);
        std::cout << STK_DATA(i);
        if (STK_DATA(i) == stack_poison_value<Data>)
          printf(" (POISON ?)");
        printf("\n");
      }
      printf("  owldata2 = %llX\n", *owldata2);
    }
    printf("  }\n");
    printf("  error = %d (%s)\n", error, error ? reason : "ok");
    printf("  owl2 = %llX\n", owl2);
    printf("}\n");
  } /* End of 'Dump' function */

private:
  /**
   * \brief Allocate memory for stack function(template)
   * \param None
   * \return true if all is OK
   * \return false otherwise.
   */
  bool DataCalloc( void )
  {
    data = (char *)calloc(maxsize * sizeof(Data) + 2 * sizeof(canary_t),
      sizeof(data[0]));

    if (data == nullptr)
    {
      error = STK_DATA_ERROR;
      return false;
    }
    FillPoi(0);
    owldata1 = (canary_t *)data;
    owldata2 = (canary_t *)((Data *)(data + sizeof(canary_t)) + maxsize);
    *owldata1 = owldata1_control;
    *owldata2 = owldata2_control;
    return true;
  } /* End of 'DataCalloc' function */

  /**
   * \brief Stack verificator function (template).
   * \return true if all is ok.
   * \return false otherwise.
   */
  bool StackOk( void )
  {
    #define STACK_COND_CHECK(COND, ERR_CODE) if (COND)                          \
                                         {                                  \
                                           error = ERR_CODE;                \
                                           return false;                    \
                                         }

    if (this == nullptr)
      return false;

    STACK_COND_CHECK(data == nullptr, STK_DATA_ERROR);

    STACK_COND_CHECK(size < 0, STK_UNDERFLOW);

    STACK_COND_CHECK(size > maxsize, STK_OVERFLOW);

    STACK_COND_CHECK(owl1 != owl1_control, STK_OWL1_ERROR);

    STACK_COND_CHECK(owl2 != owl2_control, STK_OWL2_ERROR);

    assert(owldata1 != nullptr);
    STACK_COND_CHECK(*(owldata1) != owldata1_control, STK_OWLDATA1_ERROR);

    assert(owldata2 != nullptr);
    STACK_COND_CHECK(*(owldata2) != owldata2_control, STK_OWLDATA2_ERROR);

    hash_t prev_hash = hash;
    STACK_COND_CHECK(HashReCalc() != prev_hash, STK_HASH_ERROR);

    STACK_COND_CHECK(CountPoi(size) != maxsize - size,
      STK_POI_ERROR);
    return true;

#undef STACK_COND_CHECK
  } /* End of 'StackOk' function */

  /**
   * \brief Fill stack by poison values function (template).
   * \param [in]      num    Start number to fill.
   * \return None.
   */
  void FillPoi( size_t num )
  {
    assert(this != nullptr);

    for (size_t i = num; i < (size_t)maxsize; i++)
      STK_DATA(i) = stack_poison_value<Data>;
  } /* End of 'FillPoi' function */

  /**
   * \brief Count poison values in stack function (template).
   * \param [in]       num    Start number to count.
   * \return Amount of poison values.
   */
  unsigned CountPoi( size_t num ) const
  {
    assert(this != nullptr);
    unsigned poi_cnt = 0;

    for (size_t i = num; i < (size_t)maxsize; i++)
      poi_cnt += STK_DATA(i) == stack_poison_value<Data>;

    return poi_cnt;
  } /* End of 'CountPoi' function */

  /**
   * Processing errors in stack fucntion (template).
   * \param [in]  error_code  Error code value.
   * \return None.
   */
  void Stack_Process_Error( const char filename[], int line, const char funcname[] ) const
  {
    #define STACK_DUMP_CASE(ERR_CODE, STR)  case ERR_CODE:                                      \
                                             Dump(STR, filename, line, funcname);   \
                                             break;

    if (this == nullptr)
    {
      Dump("", STACK_LOCATION);
      return;
    }
    switch (error)
    {
      STACK_DUMP_CASE(STK_DATA_ERROR, "Data was nullptr");
      STACK_DUMP_CASE(STK_HASH_ERROR, "Different hashes");
      STACK_DUMP_CASE(STK_OVERFLOW, "Stack overflow");
      STACK_DUMP_CASE(STK_OWL1_ERROR, "First owl have broken");
      STACK_DUMP_CASE(STK_OWL2_ERROR, "Second owl have broken");
      STACK_DUMP_CASE(STK_SIZE_ERROR, "Incorrect size value");
      STACK_DUMP_CASE(STK_UNDERFLOW, "Stack underflow");
      STACK_DUMP_CASE(STK_NEW_SIZE_ERROR, "New size too small");
      STACK_DUMP_CASE(STK_MEM_ERROR, "No free memory for stack");
      STACK_DUMP_CASE(STK_POI_ERROR, "Poison values have broken");
      STACK_DUMP_CASE(STK_OWLDATA1_ERROR, "First data owl have broken");
      STACK_DUMP_CASE(STK_OWLDATA2_ERROR, "Second data owl have broken");

    default:
      printf("Unrecognized error code = %d\n", error);
      break;
    }

#undef STACK_DUMP_CASE
  } /* End of 'Stack_Process_Error' function */

  /**
   * \brief Stack resize function (template).
   * \param [in]  new_size New size value.
   * \return true if all is ok.
   * \return false otherwise.
   */
  bool Resize( size_t new_size )
  {
    STK_ASSERT();
    if (new_size < stack_start_size)
    {
      error = STK_NEW_SIZE_ERROR;
      //STK_ASSERT();
      return false;
    }
    char *new_mem = (char *)realloc(data, new_size * sizeof(Data) +
      2 * sizeof(canary_t));
    if (new_mem == nullptr)
    {
      error = STK_MEM_ERROR;
      //STK_ASSERT();
      return false;
    }

    data = new_mem;
    maxsize = new_size;
    FillPoi(size);
    owldata2 = ((canary_t *)((Data *)(data + sizeof(canary_t)) + maxsize));
    *owldata2 = owldata2_control;
    HashReCalc();
    STK_ASSERT();
    return true;
  } /* End of 'Resize' function */


  /**
   * \brief Stack assertion function (template).
   * \return true if all id OK.
   * \return false otherwise.
   */
  bool Assert( const char filename[], int line, const char funcname[] )
  {
    if (error != 0)
    {
      Stack_Process_Error(filename, line, funcname);
      return false;
    }
    if (!StackOk())
    {
      Stack_Process_Error(filename, line, funcname);
      return false;
    }
    return true;
  } /* End of 'Assert' function */
};

#endif /* __STACK_H_ */


