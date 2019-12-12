#include "parser.h"

#define STR(i)  ptr->get_str()[i]

#define PR_LOCATION ptr->get_line(), ptr->get_pos()

#define CHECK_SMB(symb)  (_check_ptr_type(TOK_SMB) && STR(0) == symb)

#define CHECK_STR(str)  (_check_ptr_type(TOK_STR) && StrChrCmp(str, ptr->get_string()) == 0)

#define SYNTAX_ASSERT(cond, err)    \
  if (!(cond))                      \
  {                                 \
    throw syntax_error(err, PR_LOCATION);  \
  }

/**
 * \brief get G rule function.
 */ 
ad6::node * ad6::parser::getG( const token *toks )
{
  ptr = toks;

  node *root = _getIns();
  SYNTAX_ASSERT(_check_ptr_type(TOK_NUL), "Incorrect ending of program");

  return root;
} /* End of 'getG' function */

ad6::node * ad6::parser::_getIns( void )
{
  node *act = new node(';', _getAct(), nullptr);
  node *nd = act;
  while (!_check_ptr_type(TOK_NUL))
  {
    nd->right = new node(';', _getAct(), nullptr);
    nd = nd->right;
  }

  return act;
}

/**
 * \brief Get action rule function.
 * \return pointer to node
 */
ad6::node * ad6::parser::_getAct( void )
{
  node *nd = nullptr;
  if (CHECK_SMB('$'))
  {
    ptr++;
    return _getFunc();
  }
  return _getAss();
} /* End of '_getAct' function */

/**
 * \brief Get function rule function
 */
ad6::node * ad6::parser::_getFunc( void )
{
  string name(ptr->get_string());
  ptr++;
  node *args = _getArgs();

  SYNTAX_ASSERT(CHECK_SMB('{'), "Left brace for function body wasn't find.");
  ptr++;

  node *fnc = new node(TYPE_USR_FNC, name, 0, args, _getOp());

  SYNTAX_ASSERT(CHECK_SMB('}'), "Right brace for function body wasn't find.");
  ptr++;

  return fnc;
} /* End of '_getFunc' function */

/**
 * \brief Get operator rule function.
 */ 
ad6::node * ad6::parser::_getOp( void )
{
  if (_check_ptr_type(TOK_SMB))
  {
    SYNTAX_ASSERT(CHECK_SMB('{'), "Incorrect separating symbol");
    ptr++;
    node *op = nullptr;
    node *ops = op;

    while (!_check_ptr_type(TOK_SMB))
    {
      ops = _getOp();
      if (ops != nullptr)
        ops = ops->right;
    }

    SYNTAX_ASSERT(CHECK_SMB('}'), "Incorrect separating symbol");
    ptr++;

    return op;
  }

  if (!_check_ptr_type(TOK_STR))
    return nullptr;
  
  if (StrChrCmp("if", ptr->get_string()))
  {
    ptr++;
    SYNTAX_ASSERT(CHECK_SMB('('), "No condition for if operator");
    ptr++;

    SYNTAX_ASSERT(CHECK_SMB('('), "No condition for if operator");
    ptr++;
  }

  if (StrChrCmp("while", ptr->get_string()))
  {
  }
} /* End of '' function */


/**
 * \brief Get arguments rule function
 */
ad6::node * ad6::parser::_getArgs( void )
{
  SYNTAX_ASSERT(CHECK_SMB('('), "Left brace wasn't find in function declaration");
  ptr++;
  
  node *args = nullptr;

  if (_check_ptr_type(TOK_STR))
  {
    args = new node(',', nullptr, _getId());
    node *arg2 = args;
    while (CHECK_SMB(','))
    {
      ptr++;
      arg2->left = new node(',', nullptr, _getId());
      arg2 = arg2->left;
    }
  }

  SYNTAX_ASSERT(CHECK_SMB(')'), "Left brace wasn't find in function declaration");
  ptr++;

  return args;
} /* End of '_getArgs' function */

/*
 */
bool ad6::parser::_check_ptr_type( tok_type check )
{
  return ptr->get_type() == check;
} /* End of '_check_ptr_type' function */

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
  if (_check_ptr_type(TOK_NUM) || CHECK_SMB('-'))
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

  string str(ptr->get_string()); 
  ptr++;
  #define DEF_FNC(name, num, diff, calc)                                          \
  else if (StrChrCmp(#name, str) == 0)                                          \
  {                                                                             \
    SYNTAX_ASSERT(CHECK_SMB('('), "function '"#name"' without braces");            \
    ptr++;                                                                      \
    node *nd = _getE();                                                         \
    SYNTAX_ASSERT(CHECK_SMB(')'), "function '"#name"' without braces");            \
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
    return new node(TYPE_VAR, str, (size_t)num);
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
 * \brief Get assignment rule function.
 */
ad6::node * ad6::parser::_getAss( void )
{
  SYNTAX_ASSERT(_check_ptr_type(TOK_STR), "Invalid assignment");
  node *var = _getId();
  
  SYNTAX_ASSERT(CHECK_SMB('='), "Invalid assignment");
  ptr++;

  node *expr = _getE();

  return new node('=', var, expr);
} /* End of '_getAss' function */

ad6::node * ad6::parser::_getIf( void )
{
}

ad6::node * ad6::parser::_getWhile( void )
{
}

ad6::node * ad6::parser::_getRet( void )
{
}

ad6::node * ad6::parser::_getDec( void )
{

}

#undef cVal

#undef cVal2