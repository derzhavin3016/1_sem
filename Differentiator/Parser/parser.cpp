#include "parser.h"

#define PR_LOCATION __LINE__, __FILE__, __FUNCSIG__

#define STR(i)  ptr->get_str()[i]

#define CHECK_SMB(symb)  (_check_ptr_type(TOK_SMB) && STR(0) == symb)

#define CHECK_STR(str)  (_check_ptr_type(TOK_STR) && StrChrCmp(str, ptr->get_string()) == 0)

#define SYNTAX_ASSERT(cond, err)    \
  if (!(cond))                      \
  {                                 \
    throw Error(err, PR_LOCATION);  \
  }




/**
 * \brief get G rule function.
 */ 
ad6::node * ad6::parser::getG( const token *str )
{
  ptr = str;

  node *root = _getE();
  SYNTAX_ASSERT(_check_ptr_type(TOK_NUL), "Incorrect ending of program");

  return root;
} /* End of 'getG' function */

/**
 * \brief Get expression's rule function.
 */
ad6::node * ad6::parser::_getE( void )
{
  node *val = _getT();

  while (CHECK_SMB('+') || CHECK_SMB('-'))
  {
    char op = STR(0);
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

  while (CHECK_SMB('*') || CHECK_SMB('/'))
  {
    char op = STR(0);

    ptr++;
    node *val2 = _getPow();

    if (op == '*')
      val = &(*val * *val2);
    else
      val = &(*val / *val2);
  }

  return val;
} /* End of 'getT' function */


/**
 * \brief Check type of ptr variable.
 * \param [in]  check    Type of a token to check.
 * \return true if types are equal
 * \return false otherwise.
 */
bool ad6::parser::_check_ptr_type( tok_type check )
{
  return ptr->get_type() == check;
} /* End of '_check_ptr_type' function */

/**
 * \brief Get power rule function.
 */
ad6::node * ad6::parser::_getPow( void )
{
  node *val = _getP();

  while (CHECK_SMB('^'))
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
  if (CHECK_SMB('('))
  {
    ptr++;
    nd = _getE();
    SYNTAX_ASSERT(CHECK_SMB(')'), "Pair brace wasn't find");
    ptr++;
    return nd;
  }
  if (_check_ptr_type(TOK_NUM) || (ptr + 1)->get_type() == TOK_NUM)
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
  if (CHECK_SMB('-'))
  {
    IsNeg = true;
    ptr++;
  }
  val = ptr->get_value();

  if (IsNeg)
    val = -val;
  ptr++;
  return new node(val);
} /* End of 'getN' function */

/**
 * \brief Get variable function.
 */
ad6::node * ad6::parser::_getId( void )
{
  int pos = 0;
  node *Id = nullptr;
  if (CHECK_SMB('-'))
  {
    ptr++;
    Id = new node('*', new node(-1.0), nullptr);
  }

  string str(ptr->get_string()); 
  ptr++;
#define CHECK_ID\
    if (Id != nullptr)                                                              \
    {                                                                               \
      Id->right = fin;                                                              \
      return Id;                                                                    \
    }                                                                               \
  
  node *nd2 = nullptr;

  #define DEF_FNC(name, num, diff, calc)                                            \
  else if (StrChrCmp(#name, str) == 0)                                              \
  {                                                                                 \
    SYNTAX_ASSERT(CHECK_SMB('('), "function '"#name"' without braces");             \
    ptr++;                                                                          \
    node *nd = _getE();                                                             \
    SYNTAX_ASSERT(CHECK_SMB(')'), "function '"#name"' without braces");             \
    ptr++;                                                                          \
    node *fin = new node(TYPE_FUNC, str.str_ptr(), str.size(), num, nd2, nd);   \
    CHECK_ID;                                                                       \
    return fin;                                                                     \
  }
  
  if (0);

  #include "..\func.h"

  else
  {
    
    int num = variables.find(var(str));
    if (num == -1)
    {
      num = variables.size();
      variables.add(var(str));
    }

    node *fin = new node(TYPE_VAR, str.str_ptr(), str.size(), (size_t)num);
    CHECK_ID;
    return fin;
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
    if (StrChrCmp(str, variables[(int)i].name) == 0)
      return (int)i;

  return -1;
} /* End of 'find_var' function */

ad6::var::var( void )
{
}

ad6::var::var( string &st ) : name(st),
                    value(0)
{
}

ad6::var::var( const var &v ) : name(v.name),
                      value(v.value)
{
}

ad6::var::var( string &st, double val ) : name(st),
                                value(val)
{
}

ad6::var & ad6::var::operator=( const var &v )
{
  name = v.name;
  value = v.value;

  return *this;
}

bool ad6::var::operator==( const var &v )
{
  return name == v.name;
}


#undef cVal

#undef cVal2