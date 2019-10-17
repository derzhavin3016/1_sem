#ifndef __INPUT_H_
#define __INPUT_H_

#include <stdio.h>
#include <iostream>
#include <assert.h>

const int MAXTRIES = 50;

void ClearBuffer( void );

/**
 * \brief Input numbers function
 * \param [in]    promt[]  String to print.
 * \param [in]    promt[]  String to print before every input.
 * \param [in]    scanf_str String to scanf.
 * \param [out]  *values   Pointer to array with inputed values.
 * \param [in]    amount   Amount of values (1 default).
 * \return 1 if all is ok, 0 otherwise.
 */
template <typename Data>
int InputNumbers( const char promt[], const char promt_rep[], const char scanfstr[], Data *values, size_t amount = 1 )
{
  assert(values != NULL);
  assert(amount != 0);
 
  int correct_input = 0, is_prev_correct = 1, incr = 0;

  printf(promt);

  for (int i = 0; i < MAXTRIES && correct_input < (int)amount; i++)
  {

    if (is_prev_correct)
    {
      printf(promt_rep, correct_input + 1, amount);
      is_prev_correct = !is_prev_correct;
    }
    incr = scanf(scanfstr, values + correct_input);
    ClearBuffer();
    if (incr)
    {
      correct_input++;
      is_prev_correct = !is_prev_correct;
    }
    else
      printf("Incorrect input. Try again.\n");
  }

  return (correct_input == amount);
} /* End of 'InputNumbers' function */

/**
 * Input one value template fucntion.
 * \param [in]  promt     Promt string to print.
 * \param [in]    scanf_str String to scanf.
 * \param [out] value     Pointer to value to input.
 * \return 1 if all is OK, 0 otherwise.
 */
template <typename T>
int Input( const char promt[], const char scanfstr[], T *value )
{
  if (promt == NULL || scanfstr == NULL || value == NULL)
    return 0;

  printf(promt);
  return scanf(scanfstr, value);
} /* End of 'Input' function */

#endif /* __INPUT_H_ */
