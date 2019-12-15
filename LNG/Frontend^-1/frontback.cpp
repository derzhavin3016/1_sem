#include "frontback.h"

// class constructor
ad6::fback::fback( void ) : tree()
{
}

/**
 * \brief Read tree from file function
 * \param [in] filename Name of a file to read from.
 */
void ad6::fback::read_tree( const char filename[] )
{
  FB_ASSERT(filename != nullptr, "Incorrect file name");

  buf = FillBuf(filename, &buf_size);

  TREE_ASSERT(buf != nullptr, "Error with opening file")  root = new node;
  
  buf_ptr = buf;

  _build_tree(&root);
} /* End of 'read_tree' function */

// class destructor
ad6::fback::~fback( void )
{
}

/**
 * \brief Build tree from file function
 * \param [in] root Pointer to tree root;
 */
void ad6::fback::_build_tree( node **nd )
{
#define SWTCH(nde)                                    \
  switch (*buf_ptr)                                  \
  {                                                  \
  case '@':                                          \
    (*nd)->##nde = nullptr;                          \
    buf_ptr++;                                       \
    break;                                           \
  case '{':                                          \
    _build_tree(&((*nd)->##nde));                    \
    break;                                           \
  case '}':                                          \
    buf_ptr++;                                       \
    return;                                          \
  }

  FB_ASSERT(nd != nullptr, "node was nullptr");
  
  size_t tok_size = 0;

  sscanf(buf_ptr, "{%*[^@{}]%n", &tok_size);
  tok_size--;
  buf_ptr++;
  FB_ASSERT(tok_size != 0, "Incorrect tree file format");

  if (isdigit(*buf_ptr) || isdigit(buf_ptr[1]))
  {
    double value = 0;
    sscanf(buf_ptr, "%lg", &value);
    *nd = new node(value);
    buf_ptr += tok_size + 1;
    return;
  }
  else
    *nd = _check_buf_ptr(tok_size);

  buf_ptr += tok_size;

  SWTCH(left);
  SWTCH(right);
  buf_ptr++;
} /* End of '_build_tree' function */

/**
 * \brief Check token in buf_ptr and create a node function.
 * \param [in] tok_size  token name size
 * \return reference to new node.
 */
ad6::node * ad6::fback::_check_buf_ptr( size_t tok_size )
{
  node *res = new node;
  string name(buf_ptr, tok_size);
  if (tok_size == 1)
  {
    node_type tpe = _check_one_smb(*buf_ptr);
    
    int num = 0;
    if (tpe == TYPE_VAR)
    {
      num = variables.find(name);

      if (num == -1)
      {
        variables.add(name);
        num = variables.size() - 1;
      }
      res->set_node(TYPE_VAR, string(buf_ptr, tok_size), num);
      return res;
    }
    if (tpe == TYPE_OPERATOR)
    {
      res->set_node(TYPE_OPERATOR, "", 0, *buf_ptr);
      return res;
    }

    res->set_node(tpe, name, 0);
    return res;
  }
  if (*buf_ptr == '$')
  {
    res->set_node(TYPE_USR_FNC, string(buf_ptr + 1, tok_size - 1), 0);
    return res;
  }
  node_type tpe = _check_ops(buf_ptr, tok_size);

  int num = 0;

  if (tpe == TYPE_VAR)
  {
    num = variables.find(name);

    if (num == -1)
    {
      variables.add(name);
      num = variables.size() - 1;
    }
    res->set_node(TYPE_VAR, name, num);
    return res;
  }
   
  res->set_node(tpe, name, 0);
  return res;
} /* End of '_check_buf_ptr' function */


ad6::node_type ad6::fback::_check_ops( const char *str, size_t size )
{
#define STRCMP(str, type)                   \
  if (ad6::StrChrCmp(#str, opr) == 0)       \
     return ad6::##type;

  size_t ops_size = sizeof(ad6::pol_op) / sizeof(ad6::oper);
  ad6::string opr(str, size);

  for (size_t i = 0; i < ops_size; i++)
    if (ad6::StrChrCmp(ad6::pol_op[i].get_name(), opr) == 0)
      return ad6::TYPE_POL_OP;

  STRCMP(>=, TYPE_CMP);
  STRCMP(<=, TYPE_CMP);
  STRCMP(!=, TYPE_CMP);
  STRCMP(==, TYPE_CMP);

  STRCMP(op, TYPE_SEP);

#define DEF_FNC(name, num, code, code2)               \
  else if (ad6::StrChrCmp(#name, opr) == 0)           \
    return ad6::TYPE_FUNC;

  if (0);

#include "..\func.h"


#undef DEF_FNC

  return TYPE_VAR;
}

ad6::node_type ad6::fback::_check_one_smb( char smb )
{
  size_t size = sizeof(smb_op) / sizeof(oper);

  for (size_t i = 0; i < size; i++)
    if (smb == smb_op[i].get_num())
      return TYPE_OPERATOR;
  if (smb == '=')
    return TYPE_OPERATOR;

  if (smb == ';' || smb == ',')
    return TYPE_SEP;
  if (smb == '>' || smb == '<')
    return TYPE_CMP;
  FB_ASSERT(isalpha(smb), "Unrecognized symbol");
 
  return TYPE_VAR;
}