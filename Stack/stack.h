#ifndef __stack_h_

#include <stdio.h>
#include <iostream>
#include <assert.h>

#define stack_location __FILE__, __LINE__, __FUNCSIG__

const size_t stack_max_size = 100;

typedef unsigned long long canary_t;

const canary_t owl1_control = 0xDEAD;
const canary_t owl2_control = 0xBEDA;

template <typename Data>
const Data stack_poison_value = -6699;

template <typename Data>
struct Stack
{
  canary_t owl1;
  Data data[stack_max_size];
  int size, realsize;
  bool IsOk;
  unsigned int hash;
  canary_t owl2;
};

enum
{
  STK_UNDERFLOW = -1
};

/**
 * \brief Stack initialization function (template).
 * \param this_  Pointer to stack structure.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackInit( Stack<Data> *this_ )
{
  assert(this_ != nullptr);

  this_->owl1 = owl1_control;
  this_->size = 0;
  StackFillPoi(this_, 0);
  this_->IsOk = true;
  this_->hash = 0;
  this_->owl2 = owl2_control;
  this_->hash = StackHashCalc(this_);

  assert(StackOk(this_));
  return true;
} /* End of 'StackInit' function */

/**
 * \brief Push value to stack function (template).
 * \param this_  Pointer to stack structure.
 * \param value Value to push.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackPush( Stack<Data> *this_, Data value )
{
  assert(StackOk(this_));

  if (this_->size >= stack_max_size)
    return false;

  this_->data[this_->size++] = value;
  this_->hash = StackHashCalc(this_);

  assert(StackOk(this_));
  return true;
} /* End of 'StackPush' function */

/**
 * \brief Pop value from stack function (template).
 * \param this_  Pointer to stack structure.
 * \param err    Pointer to error code (nullptr default).
 * \return Value from the top of the stack.
 */
template <typename Data>
Data StackPop( Stack<Data> *this_, int *err = nullptr )
{
  assert(StackOk(this_));

  if (this_->size < 0)
  {
    if (err != nullptr)
      *err = STK_UNDERFLOW;
    return 0;
  }

  Data value = this_->data[--this_->size];
  StackFillPoi(this_, this_->size);
  this_->hash = StackHashCalc(this_);
  assert(StackOk(this_));
  return value;
} /* End of 'StackPop' function */

/**
 * \brief Stack destructor function (template).
 * \param this_  Pointer to stack structure.
 * \return None.
 */
template <typename Data>
void StackClose( Stack<Data> *this_ )
{
  assert(StackOk(this_));

  this_->size = 0;
} /* End of 'StackClose' function */

/**
 * \brief Stack verificator function (template).
 * \param this_  Pointer to stack structure.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackOk( Stack<Data> *this_ )
{
  if (this_ == nullptr)
  {
    StackDump(this_, "Pointer to stack was nullptr", stack_location);
    return false;
  }

  if (this_->size < 0)
  {
    this_->IsOk = false;
    StackDump(this_, "Stack underflow", stack_location);
    return false;
  }

  if (this_->size > stack_max_size)
  {
    this_->IsOk = false;
    StackDump(this_, "Stack overflow", stack_location);
    return false;
  }

  if (this_->owl1 != owl1_control)
  {
    this_->IsOk = false;
    StackDump(this_, "First owl broke", stack_location);
    return false;
  }

  if (this_->owl2 != owl2_control)
  {
    this_->IsOk = false;
    StackDump(this_, "Second owl broke", stack_location);
    return false;
  }

  unsigned prev_hash = this_->hash;
  this_->hash = 0;
  if ((this_->hash = StackHashCalc(this_)) != prev_hash)
  {
    this_->IsOk = false;
    StackDump(this_, "Differnet hashes", stack_location);
    return false;
  }
  return true;
} /* End of 'StackOk' function */

/**
 * \brief Fill stack by poison values function (template).
 * \param this_  Pointer to stack structure.
 * \param num    Start number to fill.
 * \return true None.
 */
template <typename Data>
void StackFillPoi( Stack<Data> *this_, size_t num )
{
  assert(this_ != nullptr);

  for (size_t i = num; i < stack_max_size; i++)
    this_->data[i] = stack_poison_value<Data>;
} /* End of 'StackFillPoi' function */

/**
 * \brief Stack dump function (template).
 * \param this_  Pointer to stack structure.
 * \param reason Dump reason.
 * \param filename Name of a file where Dump function called.
 * \param line Line where Dump function called.
 * \param funcname Name of a function where Dump function called.
 * \return None.
 */
template <typename Data>
void StackDump( Stack<Data> *this_, const char reason[], const char filename[],
  int line, const char funcname[] )
{

  printf("Dump (%s) from %s (%d) %s\n", reason, filename, line, funcname);
  if (this_ == nullptr)
  {
    printf("Stack [0x%p] (%s)\n{\n}", this_, "ERROR!!!");
    return;
  }
  printf("Stack [0x%p] (%s)\n", this_, this_->IsOk ? "ok" : "ERROR!!!");

  printf("{\n");
  printf("  owl1 = %llX\n", this_->owl1);
  printf("  size = %d\n", this_->size);
  printf("  data[%d]\n", stack_max_size);
  printf("  {\n");
  for (int i = 0; i < stack_max_size; i++)
  {
    printf("  %c [%d] = ", i < this_->size ? '*' : ' ', i);
    std::cout << this_->data[i] << std::endl;
  }
  printf("  }\n");
  printf("  error = %d\n", 1 - this_->IsOk);
  printf("  owl2 = %llX\n", this_->owl2);
  printf("}\n");
} /* End of 'StackDump' function */

/**
 * \brief Stack hash calculation function (template).
 * \param this_  Pointer to stack structure.
 * \return Calculated hash.
 */
template <typename Data>
unsigned StackHashCalc( Stack<Data> *this_ )
{
  assert(this_ != nullptr);
  this_->hash = 0;
  const unsigned char *bytes = (unsigned char *)this_;
  size_t bytes_len = sizeof(*this_);
  unsigned hash = 0;

  for (size_t i = 0; i < bytes_len; i++)
    hash += i * (bytes[i] << (i % 4));

  return hash;
} /* End of 'StackHashCalc' function */


#endif /* __stack_h_ */


