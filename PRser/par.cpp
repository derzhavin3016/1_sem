#include "par.h"

int ad6::parser::getG( const char *str )
{
  ptr = str;

  int val = getE();
  assert(*ptr == 0);

  return val;
}

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
}

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
      val /= val2;

  }

  return val;
}

int ad6::parser::getP( void )
{
  int val = 0;

  if (*ptr == '(')
  {
    ptr++;
    val = getE();
    assert(*ptr == ')');
    ptr++;
    return val;
  }
  return getN();
}

int ad6::parser::getN( void )
{
  int val = 0;

  do
  {
    val = val * 10 + *ptr - '0';
    ptr++;
  } while (*ptr >= '0' && *ptr <= '9');

  return val;
}
