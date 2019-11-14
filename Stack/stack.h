#ifndef __stack_h_
#define __stack_h_


#include "input.h"
#pragma warning (disable: 4996)

#define STACK_LOCATION __FILE__, __LINE__, __FUNCSIG__
#define var_name(var) #var

#define STK_START_PROMT "****************************************\n" \
                        "****************************************\n" \
                        "      Working with stack program        \n" \
                        "***********   MENU   *******************\n" \
                        "*** 0. Exit                             \n" \
                        "*** 1. Push value to stack              \n" \
                        "*** 2. Pop value from stack              \n" \
                        "*** 3. Stack dump                       \n" \
                        "****************************************\n"

#define STACK_COND_CHECK(COND, ERR_CODE) if (COND)                          \
                                         {                                  \
                                           this_->error = ERR_CODE;         \
                                           return false;                    \
                                         } 

#ifndef STK_OFF_SEC
  #define STK_ASSERT() assert(StackAssert(this_, STACK_LOCATION))
#else
  #define STK_ASSERT()
#endif

#define STK_DATA(i) ((Data *)(this_->data + sizeof(canary_t)))[i]

const size_t stack_start_size = 6;
const size_t stack_delta = 2;

typedef unsigned long long canary_t;
typedef unsigned long long hash_t;

const canary_t owl1_control = 0xDEADBEEF;
const canary_t owl2_control = 0xBEDAFADE;
const canary_t owldata1_control = 0xBEDADEAD;
const canary_t owldata2_control = 0xCCAAFFBB;

template <typename Data>
const Data stack_poison_value = 0;

template <typename Data>
struct Stack
{
  canary_t owl1;
  char *data;
  canary_t *owldata1, *owldata2;
  int size, maxsize;
  int error;
  hash_t hash;
  canary_t owl2;
};

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

#ifndef STK_OFF_SEC
/**
 * \brief Stack hash calculation function (template).
 * \param [in, out] this_  Pointer to stack.
 * \return Calculated hash.
 * \warning This function also write new hash value to stack structure.
 */
template <typename Data>
hash_t StackHashReCalc( Stack<Data> *this_ )
{
  assert(this_ != nullptr);
  
  this_->hash = 0;
  this_->hash = HashCalc(this_) + 
                HashCalc(this_->data, this_->maxsize * sizeof(Data) + 2 * sizeof(canary_t));

  return this_->hash;
} /* End of 'StackHashReCalc' function */
#else
#define StackHashReCalc(a) true;
#endif

/**
 * \brief Allocate memory for stack function(template)
 * \param this_ Pointer to stack structure.
 * \return true if all is OK
 * \return false otherwise.
 */
template <typename Data>
bool StackDataCalloc( Stack<Data> *this_ )
{
  this_->data = (char *)calloc(this_->maxsize * sizeof(Data) + 2 * sizeof(canary_t),
                               sizeof(this_->data[0]));

  if (this_->data == nullptr)
  {
    this_->error = STK_DATA_ERROR;
    return false;
  }
  StackFillPoi(this_, 0);
  this_->owldata1 = (canary_t *)(this_->data);
  this_->owldata2 = (canary_t *)((Data *)(this_->data + sizeof(canary_t)) + this_->maxsize);
  *(this_->owldata1) = owldata1_control;
  *(this_->owldata2) = owldata2_control;
  return true;
} /* End of 'StackDataCalloc' function */

/**
 * \brief Stack initialization function (template).
 * \param [in, out] this_  Pointer to stack structure.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackInit( Stack<Data> *this_ )
{
  assert(this_ != nullptr);


  this_->owl1 = owl1_control;
  this_->size = 0;
  this_->maxsize = stack_start_size;
  this_->error = 0;
  this_->hash = 0;
  this_->owl2 = owl2_control;
  if (!StackDataCalloc(this_))
  {
    STK_ASSERT();
    return false;
  }
  StackHashReCalc(this_);

  STK_ASSERT();
  return true;
} /* End of 'StackInit' function */

/**
 * \brief Push value to stack function (template).
 * \param [in, out] this_ Pointer to stack structure.
 * \param [in]      value Value to push.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackPush( Stack<Data> *this_, Data value )
{
  STK_ASSERT();

  if (this_->size >= this_->maxsize)
    if (!StackResize(this_, this_->maxsize * 2))
    {
      STK_ASSERT();
      return false;
    }

  STK_DATA(this_->size++) = value;
  StackHashReCalc(this_);

  STK_ASSERT();
  return true;
} /* End of 'StackPush' function */

/**
 * \brief Pop value from stack function (template).
 * \param [in, out] this_  Pointer to stack structure.
 * \param [out]     err    Pointer to return a value.
 * \return true if all is OK.
 * \return false otherwise.
 */
template <typename Data>
bool StackPop( Stack<Data> *this_, Data *value )
{
  STK_ASSERT();

  if (this_->size == 0)
  {
    this_->error = STK_UNDERFLOW;
    STK_ASSERT();
    return false;
  }
  if (this_->maxsize > stack_start_size &&
    this_->size == this_->maxsize / 2 - stack_delta)
    if (!StackResize(this_, this_->maxsize / 2))
    {
      STK_ASSERT();
      return false;
    }

  *value = STK_DATA(--this_->size);
  StackFillPoi(this_, this_->size);
  StackHashReCalc(this_);
  STK_ASSERT();
  return true;
} /* End of 'StackPop' function */

/**
 * \brief Stack destructor function (template).
 * \param [in, out] this_  Pointer to stack structure.
 * \return None.
 */
template <typename Data>
void StackClose( Stack<Data> *this_ )
{
  STK_ASSERT();

  free(this_->data);
  this_->size = 0;
} /* End of 'StackClose' function */

/**
 * \brief Stack verificator function (template).
 * \param [in, out] this_  Pointer to stack structure.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackOk( Stack<Data> *this_ )
{
  if (this_ == nullptr)
    return false;

  STACK_COND_CHECK(this_->data == nullptr, STK_DATA_ERROR);

  STACK_COND_CHECK(this_->size < 0, STK_UNDERFLOW);

  STACK_COND_CHECK(this_->size > this_->maxsize, STK_OVERFLOW);

  STACK_COND_CHECK(this_->owl1 != owl1_control, STK_OWL1_ERROR);

  STACK_COND_CHECK(this_->owl2 != owl2_control, STK_OWL2_ERROR);

  assert(this_->owldata1 != nullptr);
  STACK_COND_CHECK(*(this_->owldata1) != owldata1_control, STK_OWLDATA1_ERROR);
  
  assert(this_->owldata2 != nullptr);
  STACK_COND_CHECK(*(this_->owldata2) != owldata2_control, STK_OWLDATA2_ERROR);

#ifndef STK_OFF_SEC
  hash_t prev_hash = this_->hash;
  STACK_COND_CHECK(StackHashReCalc(this_) != prev_hash, STK_HASH_ERROR);
#endif
  STACK_COND_CHECK(StackCountPoi(this_, this_->size) != this_->maxsize - this_->size, 
                   STK_POI_ERROR);
  return true;

} /* End of 'StackOk' function */

/**
 * \brief Fill stack by poison values function (template).
 * \param [in, out] this_  Pointer to stack structure.
 * \param [in]      num    Start number to fill.
 * \return None.
 */
template <typename Data>
void StackFillPoi( Stack<Data> *this_, size_t num )
{
  assert(this_ != nullptr);

  for (size_t i = num; i < (size_t)this_->maxsize; i++)
    STK_DATA(i) = stack_poison_value<Data>;
} /* End of 'StackFillPoi' function */

/**
 * \brief Count poison values in stack function (template).
 * \param [in, out]  this_  Pointer to stack structure.
 * \param [in]       num    Start number to count.
 * \return Amount of poison values.
 */
template <typename Data>
unsigned StackCountPoi( Stack<Data> *this_, size_t num )
{
  assert(this_ != nullptr);
  unsigned poi_cnt = 0;

  for (size_t i = num; i < (size_t)this_->maxsize; i++)
    poi_cnt += STK_DATA(i) == stack_poison_value<Data>;

  return poi_cnt;
} /* End of 'StackCountPoi' function */

/**
 * Processing errors in stack fucntion (template).
 * \param [in]  error_code  Error code value.
 * \return None.
 */
template <typename Data>
void Stack_Process_Error( Stack<Data> *this_, const char filename[],
                          int line, const char funcname[] )
{
  #define STACK_DUMP_CASE(ERR_CODE, STR)  case ERR_CODE:                                      \
                                           StackDump(this_, STR, filename, line, funcname);   \
                                           break;

  if (this_ == nullptr)
  {
    StackDump(this_, "", STACK_LOCATION);
    return;
  }
  switch (this_->error)
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
    printf("Unrecognized error code = %d\n", this_->error);
    break;
  }
#undef STACK_DUMP_CASE
} /* End of 'Stack_Process_Error' function */

/**
 * \brief Stack dump function (template).
 * \param [in, out] this_  Pointer to stack structure.
 * \param [in]  reason Dump reason.
 * \param [in]  filename Name of a file where Dump function called.
 * \param [in]  line Line where Dump function called.
 * \param [in]  funcname Name of a function where Dump function called.
 * \return None.
 */
template <typename Data>
void StackDump( Stack<Data> *this_, const char reason[], const char filename[],
                int line, const char funcname[] )
{
  if (this_ == nullptr)
  {
    printf("Dump (%s) from %s (%d) %s\n", "Pointer to stack was nullptr",
      filename, line, funcname);
    printf("Stack \"%s\" [0x%p] (%s)\n{\n}", var_name(this_),this_, "ERROR!!!");
    return;
  }
  printf("Dump (%s) from %s (%d) %s\n", reason, filename, line, funcname);
  printf("Stack \"%s\"[0x%p] (%s)\n", var_name(*this_), this_, StackOk(this_) ? "ok" : "ERROR!!!");

  printf("{\n");
  printf("  owl1 = %llX\n", this_->owl1);
  printf("  size = %d\n", this_->size);
  printf("  maxsize = %d\n", this_->maxsize);
  printf("  data[%d]%s\n", this_->maxsize, this_->data == nullptr ? 
    " - NULLPTR!!!" : "");
  printf("  {\n");
  if (this_->data != nullptr)
  {
    printf("  owldata1 = %llX\n", *(this_->owldata1));
    for (int i = 0; i < this_->maxsize; i++)
    {
      printf("  %c [%d] = ", i < this_->size ? '*' : ' ', i);
      std::cout << ((Data *)(this_->data + sizeof(canary_t)))[i];
      if (((Data *)(this_->data + sizeof(canary_t)))[i] == stack_poison_value<Data>)
        printf(" (POISON ?)");
      printf("\n");
    }
    printf("  owldata2 = %llX\n", *(this_->owldata2));
  }
  printf("  }\n");
  printf("  error = %d (%s)\n", this_->error, this_->error ? reason : "ok");
  printf("  owl2 = %llX\n", this_->owl2);
  printf("}\n");
} /* End of 'StackDump' function */

/**
 * \brief Hash calculation function (template).
 * \param [in, out] this_  Pointer to value to calculate hash.
 * \param [in]      size   Size of array (1 default).
 * \return Calculated hash.
 */
template <typename Data>
hash_t HashCalc( const Data *this_, size_t size = 1 )
{
  assert(this_ != nullptr);
  const unsigned char *bytes = (unsigned char *)this_;
  size_t bytes_len = sizeof(*this_) * size;
  hash_t hash = 0;

  for (size_t i = 0; i < bytes_len; i++)
    hash += (i + 1) * (bytes[i] << (i % 4));

  return hash;
} /* End of 'HashCalc' function */


/**
 * \brief Stack resize function (template).
 * \param [in, out] this_  Pointer to stack.
 * \param [in]  new_size New size value.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackResize( Stack<Data> *this_, size_t new_size )
{
  STK_ASSERT();
  if (new_size < stack_start_size)
  {
    this_->error = STK_NEW_SIZE_ERROR;
    //STK_ASSERT();
    return false;
  }
  char *new_mem = (char *)realloc(this_->data, new_size * sizeof(Data) +
    2 * sizeof(canary_t));
  if (new_mem == nullptr)
  {
    this_->error = STK_MEM_ERROR;
    //STK_ASSERT();
    return false;
  }

  this_->data = new_mem;
  this_->maxsize = new_size;
  StackFillPoi(this_, this_->size);
  this_->owldata2 = ((canary_t *)((Data *)(this_->data + sizeof(canary_t)) + this_->maxsize));
  *(this_->owldata2)  = owldata2_control;
  STK_ASSERT();
  return true;
} /* End of 'StackResize' function */


/**
 * \brief Stack assertion function (template).
 * \param [in, out] this_ Pointer to stack structure.
 * \return true if all id OK.
 * \return false otherwise.
 */
template <typename Data>
bool StackAssert( Stack<Data> *this_, const char filename[],
                  int line, const char funcname[])
{
  if (this_->error != 0)
  {
    Stack_Process_Error(this_, filename, line, funcname);
    return false;
  }
  if (!StackOk(this_))
  {
    Stack_Process_Error(this_, filename, line, funcname);
    return false;
  }
  return true;
} /* End of 'StackAssert' function */


/**
 * \brief Work with stack function (template).
 * \param [in, out] this_ Pointer to stack structure.
 * \return None.
 */
template <typename Data>
void StackProcLoop( Stack<Data> *this_ )
{
  STK_ASSERT();
  int prmt = 0;
  const char *types[4] = {"%d", "%f", "%lg", "%c"};
  int chosen_type = 0;
  /*int OK = InputNumbers("Choose stack types before start:\n"
    "1 - int\n2 - float\n3 - double\n4 - char\n", 
    "Input type number: \n", "%d", &chosen_type);
  chosen_type--;
  chosen_type = Clamp(chosen_type, 0, 3);
  assert(OK);*/
  while(1)
  {
    int OK = InputNumbers(STK_START_PROMT, "*** Input number to start:\n",
      "%d", &prmt);
    assert(OK);
    switch (prmt)
    {
    case 0:
      return;
    case 1:
      Data push;
      OK = InputNumbers("PUSH\n", "Input value to push:\n",
        types[chosen_type], &push);
      assert(OK);
      if (!StackPush(this_, push))
        STK_ASSERT();
      else
        printf("Push succeed\n");
      break;
    case 2:
      Data pop;
      if (!StackPop(this_, &pop))
        STK_ASSERT();
      else
      {
        printf("Popped value:");
        printf(types[chosen_type], pop);
        printf("\n");
      }
      break;
    case 3:
      StackDump(this_, "Just info", STACK_LOCATION);
      break;
    default:
      printf("Unrecognized switch\n");
      break;
    }
  }
}


#endif /* __stack_h_ */


