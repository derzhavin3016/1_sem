#include "parser.h"

#define PR_LOCATION __LINE__, __FILE__, __FUNCSIG__

#define SYNTAX_ASSERT(cond, err)    \
  if (!(cond))                      \
  {                                 \
    throw Error(err, PR_LOCATION);  \
  }

/**
 * \brief get G rule function.
 */ 
ad6::node * ad6::parser::getG( const char *str )
{
  ptr = str;

  node *root = _getE();
  SYNTAX_ASSERT(*ptr == '\r', "Incorrect ending of program");

  return root;
} /* End of 'getG' function */

/**
 * \brief Get expression's rule function.
 */
ad6::node * ad6::parser::_getE( void )
{
  node *val = _getT();

  while (*ptr == '+' || *ptr == '-')
  {
    char op = *ptr;
    ptr++;

    node *val2 = _getT();

    if (op == '+')
      val = &(*val + *val2);
    else
      val = &(*val - *val2);
  }

  return val;
} /* End of 'getE' function */

/**
 * \brief Get T rule function.
 */
ad6::node * ad6::parser::_getT( void )
{
  node *val = _getPow();

  while (*ptr == '*' || *ptr == '/')
  {
    char op = *ptr;

    ptr++;
    node *val2 = _getPow();

    if (op == '*')
      val = &(*val * *val2);
    else
      val = &(*val / *val2);

    int dummy = 0;
  }

  return val;
} /* End of 'getT' function */

/**
 * \brief Get power rule function.
 */
ad6::node * ad6::parser::_getPow( void )
{
  node *val = _getP();

  while (*ptr == '^')
  {
    ptr++;
    node *val2 = _getP();

    val = &(*val ^ *val2);
  }

  return val;
} /* End of 'getT' function */

/**
 * \brief Get primitive expression's rule function.
 */
ad6::node * ad6::parser::_getP( void )
{
  node *nd = nullptr;
  if (*ptr == '(')
  {
    ptr++;
    nd = _getE();
    SYNTAX_ASSERT(*ptr == ')', "Pair brace wasn't find");
    ptr++;
    return nd;
  }
  if (isdigit(*(ptr)) || *ptr == '-' )
    return _getN();
  return _getId();
} /* End of 'getP' function */

double dec_neg_pow( int deg )
{
  if (deg == 0)
    return 1;
  return dec_neg_pow(deg + 1) / 10.0;
}

/**
 * \brief Get number's rule function.
 */
ad6::node * ad6::parser::_getN( void )
{
  double val = 0;
  bool IsNeg = false;
  if (*ptr == '-')
  {
    IsNeg = true;
    ptr++;
  }
  do
  {
    val = val * 10 + *ptr - '0';
    ptr++;
  } while (*ptr >= '0' && *ptr <= '9');
  if (*ptr == '.')
  {
    ptr++;
    int deg_cnt = -1;
    do
    {
      val += (*ptr - '0') * dec_neg_pow(deg_cnt--);
      ptr++;
    } while (*ptr >= '0' && *ptr <= '9');
  }

  if (IsNeg)
    val = -val;

  return new node(val);
} /* End of 'getN' function */

/**
 * \brief Get variable function.
 */
ad6::node * ad6::parser::_getId( void )
{
  int pos = 0;
  const char *old_ptr = ptr;
  do
  {
    ptr++;
  } while (isalpha(*ptr));

  string str(old_ptr, (size_t)(ptr - old_ptr)); 

  #define DEF_FNC(name, num, diff, calc)                                          \
  else if (StrChrCmp(#name, str) == 0)                                          \
  {                                                                             \
    SYNTAX_ASSERT(*ptr == '(', "function '"#name"' without braces");            \
    ptr++;                                                                      \
    node *nd = _getE();                                                         \
    SYNTAX_ASSERT(*ptr == ')', "function '"#name"' without braces");            \
    ptr++;                                                                      \
    return new node(TYPE_FUNC, str.str_ptr(), str.size(), num, nullptr, nd);    \
  }

  if (0);

  #include "..\func.h"

  else
  {
    int num = 0;
    
    if ((num = variables.find(str)) == -1)
    {
      num = variables.size();
      variables.add(str);
    }
    return new node(TYPE_VAR, old_ptr, (size_t)(ptr - old_ptr), (size_t)num);
  }
} /* End of 'getId' function  */
    
    

/**
 * \brief Find variable in variables array by string.
 * \param [in] str  pointer to string with variable.
 * \return number of variable if variable was find.
 * \return -1 otherwise.
 */
int ad6::parser::find_var( const char str[] )
{
  for (size_t i = 0 ; i < variables.size(); i++)
    if (StrChrCmp(str, variables[i]) == 0)
      return i;

  return -1;
} /* End of 'find_var' function */

/**
 * \brief Get fucntion function
 */
ad6::node * ad6::parser::_getFunc( void )
{
  int pos = 0;
  const char *old_ptr = ptr;
  do
  {
    ptr++;
  } while (isalpha(*ptr));

  string str(old_ptr, (size_t)(ptr - old_ptr));

#define DEF_FNC(name, num, diff, calc)                                          \
  else if (StrChrCmp(#name, str) == 0)                                          \
  {                                                                             \
    SYNTAX_ASSERT(*ptr == '(', "function '"#name"' without braces");            \
    ptr++;                                                                      \
    node *nd = _getE();                                                         \
    SYNTAX_ASSERT(*ptr == ')', "function '"#name"' without braces");            \
    ptr++;                                                                      \
    return new node(TYPE_FUNC, str.str_ptr(), str.size(), num, nullptr, nd);    \
  }

  if (0);

  #include "..\func.h"

  else
    ptr = old_ptr;

 #undef DEF_FNC
  return nullptr;
} /* End of 'getFunc' function */

#undef cVal

#undef cVal2