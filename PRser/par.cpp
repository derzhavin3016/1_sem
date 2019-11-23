#include "par.h"

#define PR_LOCATION __LINE__, __FILE__, __FUNCSIG__

#define SYNTAX_ASSERT(cond, err)    \
  if (!(cond))                      \
  {                                 \
    throw Error(err, PR_LOCATION);  \
  }

/**
 * \brief get G rule function.
 */ 
int ad6::parser::getG( const char *str )
{
  ptr = str;

  int val = getE();
  SYNTAX_ASSERT(*ptr == 0, "Incorrect ending of program");

  return val;
} /* End of 'getG' function */

/**
 * \brief Get expression's rule function.
 */
int ad6::parser::getE( void )
{
  int val = getT();

  while (*ptr == '+' || *ptr == '-')
  {
    char op = *ptr;
    ptr++;

    int val2 = getT();

    if (op == '+')
      val += val2;
    else
      val -= val2;
  }

  return val;
} /* End of 'getE' function */

/**
 * \brief Get T rule function.
 */
int ad6::parser::getT( void )
{
  int val = getP();

  while (*ptr == '*' || *ptr == '/')
  {
    char op = *ptr;

    ptr++;
    int val2 = getP();

    if (op == '*')
      val *= val2;
    else
    {
      SYNTAX_ASSERT(val2 != 0, "Dividing by zero!!");
      val /= val2;
    }

  }

  return val;
} /* End of 'getT' function */

/**
 * \brief Get primitive expression's rule function.
 */
int ad6::parser::getP( void )
{
  int val = 0;

  if (*ptr == '(')
  {
    ptr++;
    val = getE();
    SYNTAX_ASSERT(*ptr == ')', "Pair brace wasn't find");
    ptr++;
    return val;
  }
  //if (isdigit(*(ptr + 1)))
    return getN();
} /* End of 'getP' function */

/**
 * \brief Get number's rule function.
 */
int ad6::parser::getN( void )
{
  int val = 0;

  do
  {
    val = val * 10 + *ptr - '0';
    ptr++;
  } while (*ptr >= '0' && *ptr <= '9');

  return val;
} /* End of 'getN' function */

/**
 * \brief Get variable function.
 */
int getId( void )
{
  
} /* End of 'getId' function  */

/**
 * \brief Get fucntion function
 */
int getFunc( void )
{
  
} /* End of 'getFunc' function */
