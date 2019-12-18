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
  node *act = new node(TYPE_SEP, ";", 1, 0, _getAct(), nullptr);
  node *nd = act;
  while (!_check_ptr_type(TOK_NUL))
  {
    nd->right = new node(TYPE_SEP, ";", 1, 0, _getAct(), nullptr);
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
    act_fnc = functions.size();
    return _getFunc();
  }
  act_fnc = GLOBAL_VAR;
  return _getDec();
} /* End of '_getAct' function */

/**
 * \brief Get function rule function
 */
ad6::node * ad6::parser::_getFunc( void )
{
  string name(ptr->get_string());
  ptr++;
  node *args = _getArgs();
  int num_of_args = 0;

  if (args != nullptr)
    num_of_args = (int)args->value;


  functions.add(fnc(name, num_of_args));

  node *fnc = new node(TYPE_USR_FNC, name, functions.size() - 1, args, _getOp());

  return fnc;
} /* End of '_getFunc' function */

/**
 * \brief get comprasion rule function
 */
ad6::node * ad6::parser::_getComp( void )
{

#define TWO_SMB_ADD(str)\
  if (CHECK_SMB(str##[0]))                                                             \
  {                                                                                \
    ptr++;                                                                         \
    SYNTAX_ASSERT(CHECK_SMB(str##[1]), "Incorrect comprasion");                    \
    ptr++;                                                                         \
    node *nd = new node(TYPE_CMP, str, 2, 0, left, _getE());                       \
    return nd;                                                                     \
  }

#define CHECK_CREATE(fst_smb, add_smb)                                             \
  if (CHECK_SMB(#add_smb[0]))                                                      \
    {                                                                              \
      ptr++;                                                                       \
      node *nd = new node(TYPE_CMP, #fst_smb#add_smb, 2, 0, left, _getE());  \
      return nd;                                                                   \
    }

#define CHECK_FST_ADD(fst, add)                                                    \
  if (CHECK_SMB(#fst[0]))                                                          \
  {                                                                                \
    ptr++;                                                                         \
    CHECK_CREATE(fst, add);                                                        \
    node *nd = new node(TYPE_CMP, #fst, 1, 0, left, _getE());                      \
    return nd;                                                                     \
  }

  node *left = _getE();

  CHECK_FST_ADD(>, =);
  CHECK_FST_ADD(<, =);
  
  TWO_SMB_ADD("!=");
  TWO_SMB_ADD("==");

  SYNTAX_ASSERT(0, "Incorrect comprasion");

#undef STR_ADD

#undef CHECK_CREATE

#undef CHECK_FST_ADD
  return nullptr;
}

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
    node **ops = &op;

    while (!_check_ptr_type(TOK_SMB))
    {
      *ops = _getOp();
      if (ops != nullptr)
        ops = &(*ops)->right;
    }

    SYNTAX_ASSERT(CHECK_SMB('}'), "Incorrect separating symbol");
    ptr++;

    return op;
  }

  if (!_check_ptr_type(TOK_STR))
    return nullptr;

 
  if (StrChrCmp(LNG_IF, ptr->get_string()) == 0)
    return new node(_getIf(), nullptr);

  if (StrChrCmp(LNG_WHILE, ptr->get_string()) == 0)
    return new node(_getWhile(), nullptr);

  if (StrChrCmp(LNG_RETURN, ptr->get_string()) == 0)
    return new node(_getRet(), nullptr);

  if (StrChrCmp(LNG_VAR, ptr->get_string()) == 0)
    return new node(_getDec(), nullptr);

  if (StrChrCmp(LNG_PUT, ptr->get_string()) == 0)
    return new node(_getPut(), nullptr);

  if (StrChrCmp(LNG_GET, ptr->get_string()) == 0)
    return new node(_getGet(), nullptr);

  node *ass = new node(_getAss(), nullptr);
  return ass;
} /* End of '_getOp' function */

/**
 * \brief Get 'Put' rule function.
 */
ad6::node * ad6::parser::_getPut( void )
{
  ptr++;
  SYNTAX_ASSERT(CHECK_SMB('('), "No left brace for put");
  ptr++;

  node *put = new node(TYPE_FUNC, "put", 3, 0, _getE());

  SYNTAX_ASSERT(CHECK_SMB(')'), "No right brace for put");
  ptr++;
  SYNTAX_ASSERT(CHECK_SMB(';'), "No ';' find for put");
  ptr++;

  return put;
} /* End of 'Put' function */

/**
 * \brief Get 'Get' rule function.
 */
ad6::node * ad6::parser::_getGet( void )
{
  ptr++;
  SYNTAX_ASSERT(CHECK_SMB('('), "No left brace for get");
  ptr++;

  node *get = new node(TYPE_FUNC, "get", 3, 0, _getE());

  SYNTAX_ASSERT(CHECK_SMB(')'), "No right brace for get");
  ptr++;
  SYNTAX_ASSERT(CHECK_SMB(';'), "No ';' find for get");
  ptr++;

  return get;
} /* End of 'Put' function */

/**
 * \brief Get arguments rule function
 */
ad6::node * ad6::parser::_getArgs( void )
{
#define NEW_VAR                                                              \
  new node(TYPE_VAR, ptr->get_string(), _var_add(ptr->get_string()))

  SYNTAX_ASSERT(CHECK_SMB('('), "Left brace wasn't find in function declaration");
  ptr++;
  
  node *args = nullptr;

  int args_cnt = 0;

  if (_check_ptr_type(TOK_STR))
  {
    args = new node(TYPE_SEP, ",", 1, 0, nullptr, NEW_VAR);
    ptr++;
    args_cnt++;
    node *arg2 = args;
    while (CHECK_SMB(','))
    {
      ptr++;
      args_cnt++;
      arg2->left = new node(TYPE_SEP, ",", 1, 0, nullptr, NEW_VAR);
      ptr++;
      arg2 = arg2->left;
    }
  }

  SYNTAX_ASSERT(CHECK_SMB(')'), "Right brace wasn't find in function declaration");
  ptr++;
  if (args != nullptr)
    args->value = args_cnt;

#undef NEW_VAR
  return args;
} /* End of '_getArgs' function */

/**
 * \brief Add new variable function
 */
unsigned ad6::parser::_var_add( const string &st )
{
  var _new(st, act_fnc);
  int num = variables.find(_new);

  SYNTAX_ASSERT(num == -1, "Variable redeclaration");

  variables.add(_new);

  return variables.size() - 1;
} /* End of '_var_add' function */

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
  if (CHECK_SMB(')'))
    return nullptr;
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

  #define DEF_FNC(name, num, diff, calc, code)                                            \
  else if (StrChrCmp(#name, str) == 0)                                              \
  {                                                                                 \
    SYNTAX_ASSERT(CHECK_SMB('('), "function '"#name"' without braces");             \
    ptr++;                                                                          \
    node *nd = _getE();                                                             \
    SYNTAX_ASSERT(CHECK_SMB(')'), "function '"#name"' without braces");             \
    ptr++;                                                                          \
    node *fin = new node(TYPE_FUNC, str.str_ptr(), str.size(), num, nullptr, nd);   \
    CHECK_ID;                                                                       \
    return fin;                                                                     \
  }

  if (0);

  #include "..\func.h"

  else
  {
    
    int num = variables.find(var(str, act_fnc));
    if (num == -1)
      num = variables.find(var(str, GLOBAL_VAR));
    if (num != -1)
    {
      node *fin = new node(TYPE_VAR, str, (unsigned)num);
      CHECK_ID;
      return fin;
    }

    node *args = _getArgsE();

    int cnt = 0;
    if (args != nullptr)
      cnt = (int)args->value;
    SYNTAX_ASSERT((num = functions.find(fnc(str, cnt)) != -1), "Incorrect function call")

    node *fin = new node(TYPE_USR_FNC, str, (unsigned)num, args);
    CHECK_ID;
    return fin;
  }
#undef CHECK_ID
} /* End of 'getId' function */

ad6::node * ad6::parser::_getArgsE( void )
{
  SYNTAX_ASSERT(CHECK_SMB('('), "Left brace wasn't find in function call");
  ptr++;
  
  node *args = nullptr;

  int args_cnt = 0;

  if (!CHECK_SMB(')'))
  {
    args = new node(TYPE_SEP, ",", 1, 0, nullptr, _getE());
    args_cnt++;
    node *arg2 = args;
    while (CHECK_SMB(','))
    {
      ptr++;
      args_cnt++;
      arg2->left = new node(TYPE_SEP, ",", 1, 0, nullptr, _getE());
      arg2 = arg2->left;
    }
  }

  SYNTAX_ASSERT(CHECK_SMB(')'), "Right brace wasn't find in function call");
  ptr++;
  if (args != nullptr)
    args->value = args_cnt;
  return args;
}

ad6::string & ad6::parser::get_var( unsigned num ) const
{
  return variables[num].get_name();  
}


/**
 * \brief Find variable in variables array by string.
 * \param [in] str  pointer to string with variable.
 * \return number of variable if variable was find.
 * \return -1 otherwise.
 */
int ad6::parser::find_var( const  char str[] )
{
  for (unsigned i = 0 ; i < variables.size(); i++)
    if (StrChrCmp(str, variables[i].get_name()) == 0)
      return i;

  return -1;
} /* End of 'find_var' function */

/**
 * \brief Get declaration rule function.
 */
ad6::node * ad6::parser::_getDec( void )
{
  SYNTAX_ASSERT(CHECK_STR(LNG_VAR), "Incorrect variable declaration");
  ptr++;
  SYNTAX_ASSERT(_check_ptr_type(TOK_STR), "Incorrect variable name");

  int num = _var_add(ptr->get_string());

  return _getAss();
} /* End of '_getDec' function */

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
  SYNTAX_ASSERT(CHECK_SMB(';'), "';' was not met");
  ptr++;

  return new node('=', var, expr);
} /* End of '_getAss' function */

/**
 * \brief Get 'if' rule function
*/
ad6::node * ad6::parser::_getIf( void )
{
  SYNTAX_ASSERT(CHECK_STR(LNG_IF), "Incorrect 'if'");
  ptr++;
  SYNTAX_ASSERT(CHECK_SMB('('), "No condition for if operator");
  ptr++;

  node *_if_else = new node(TYPE_POL_OP, "if-else", 7, 0, nullptr, nullptr); 
  node *_if = new node(TYPE_POL_OP, "if", 2, 0, _getComp(), _if_else);

  SYNTAX_ASSERT(CHECK_SMB(')'), "No condition for if operator");
  ptr++;

  _if_else->left = _getOp();
  if (CHECK_STR(LNG_ELSE))
  {
    ptr++;
    _if_else->right = _getOp();
  }
  return _if;
}

/**
 * \brief Get 'while' rule function.
 */
ad6::node * ad6::parser::_getWhile( void )
{
  SYNTAX_ASSERT(CHECK_STR(LNG_WHILE), "Incorrect 'while'");
  ptr++;
  SYNTAX_ASSERT(CHECK_SMB('('), "No condition for 'while' operator");
  ptr++;

  node *_while = new node(TYPE_POL_OP, "while", 5, 0, _getComp(), nullptr);

  SYNTAX_ASSERT(CHECK_SMB(')'), "No condition for 'while' operator");
  ptr++;

  _while->right = _getOp();
  return _while;
}

/**
 * \brief Get return rule function
 */
ad6::node * ad6::parser::_getRet( void )
{
  SYNTAX_ASSERT(CHECK_STR(LNG_RETURN), "");
  ptr++;
  node* ret = new node(TYPE_POL_OP, "ret", 3, 0, nullptr, nullptr);
  if (!CHECK_SMB(';'))
    ret->left = _getE();
  ptr++;
  return ret;
} /* End of 'Ret' fucntion */

#undef cVal

#undef cVal2