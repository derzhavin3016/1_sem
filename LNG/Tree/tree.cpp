#include "tree.h"

ad6::base_translator::base_translator( void ) : root(nullptr),
                          buf(nullptr),
                          buf_size(0),
                          tr(nullptr),
                          act_fnc(GLOBAL_VAR),
                          cnt_fnc(0)
{

}


/**
 * \brief tree dump function.
 * \param filename Name of a file to dump.
 */
bool ad6::base_translator::dump( const char filename[], node *nd )
{
  TREE_ASSERT(filename != nullptr, "Incorrect file name");

  char buf[ANSWER_MAX] = {};

  sprintf(buf, "%s.dot", filename);

  tr = fopen(buf, "w");

  if (tr == nullptr)
  {
    printf("Oh, some errors with openning file %s\n", buf);
    return false;
  }

  fprintf(tr, "digraph %s \n{\n", filename);
  
  rec_dump(nd);
  
  fprintf(tr, "}\n");
 
  fclose(tr);
 
  sprintf(buf, "dot -Tpng %s.dot -o%s.png", filename, filename);

  system(buf);
  return true;
} /* End of 'dump' function */

/**
 * \brief Recursion dump tree function.
 * \param node  Pointer to node.
 */
void ad6::base_translator::rec_dump( node *node ) const
{
  TREE_ASSERT(tr != nullptr, "File not opened");
  TREE_ASSERT(node != nullptr, "node was nullptr");
#define PUT_END(shp, fill, col)              \
  fprintf(tr, "\", shape = %s, style = %s, fillcolor = %s, color = black];\n", shp, fill, col);

#define CHECK_L                                         \
  if (node->left != nullptr)                            \
  {                                                     \
    fprintf(tr, "\"%p\" -> ", node);                    \
    rec_dump(node->left);                               \
  }

#define CHECK_R                                         \
  if (node->right != nullptr)                           \
  {                                                     \
    fprintf(tr, "\"%p\" -> ", node);                    \
    rec_dump(node->right);                              \
  }

#define PRINT_BR                                         \
   fprintf(tr, "\n\"%p\" [label = \"", node);           \
    node->name.print_in_file(tr);


#define CHECK_L_R  \
  {                \
    CHECK_L;       \
    CHECK_R;       \
  }


  switch (node->type)
  {
  case TYPE_OPERATOR:
    CHECK_L_R;

    fprintf(tr, "\n\"%p\" [label = \"%c", node, node->num);
    if (node->num == '=')
      fprintf(tr, "\", shape = larrow];\n");
    else
      PUT_END("diamond", "filled", _get_op_col(node->num));


    break;
  case TYPE_NUMBER:

    fprintf(tr, "\"%p\";\n\"%p\" [label = \"%lg", node, node, node->value);
    PUT_END("egg", "filled", "darkseagreen2");
    break;
  case TYPE_VAR:
    fprintf(tr, "\"%p\";\n\"%p\" [label = \"", node, node);

    node->name.print_in_file(tr);
    PUT_END("egg", "filled", "darkolivegreen2");
    break;
  case TYPE_FUNC:
    CHECK_R;
    CHECK_L;

    fprintf(tr, "\"%p\"", node);

    PRINT_BR;
   
    PUT_END("box", "filled", "bisque");
    break;
  case TYPE_SEP:
    CHECK_L_R;

    PRINT_BR;
    fprintf(tr, "\"];\n");
    break;
  case TYPE_USR_FNC:
    CHECK_L_R;
    
    fprintf(tr, "\"%p\"", node);

    fprintf(tr, "\n\"%p\" [label = \"$", node);
    node->name.print_in_file(tr);
    PUT_END("box", "filled", "bisque");
    break;
  case TYPE_POL_OP:
    CHECK_L_R;

    fprintf(tr, "\"%p\"", node);

    PRINT_BR;
    PUT_END(_get_op_shp(node->name), "solid", "black");
    break;
  case TYPE_CMP:
    CHECK_L_R;

    PRINT_BR;
    PUT_END("parallelogram", "filled", "grey56");
    break;
  default:
    printf("Unrecognised type : %d", node->type);
    break;
  }
#undef IsLeaf

#undef CHECK_L

#undef CHECK_R

#undef CHECK_L_R

#undef PRINT_BR
} /* End of 'rec_dump' function */ 


/**
 * \brief Print tree to opened file function.
 * \param [in] node pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::base_translator::_print_tree( node *node ) const
{
  TREE_ASSERT(tr != nullptr, "File not opened\n");
  TREE_ASSERT(node != nullptr, "node was nullptr");

#define IsLeaf(node)  ((node)->right == nullptr && (node)->left == nullptr)

  fprintf(tr, "{");
  switch (node->type)
  {
  case TYPE_NUMBER:
    fprintf(tr, "%lg", node->value);
    break;
  case TYPE_FUNC:
    node->name.print_in_file(tr);
    break;
  case TYPE_OPERATOR:
    fputc(node->num, tr);
    break;
  case TYPE_SEP:
    node->name.print_in_file(tr);
    break;
  case TYPE_VAR:
    node->name.print_in_file(tr);
    break;
  case TYPE_USR_FNC:
    fputc('$', tr);
    node->name.print_in_file(tr);
    break;
  case TYPE_POL_OP:
    node->name.print_in_file(tr);
    break;
  case TYPE_CMP:
    node->name.print_in_file(tr);
    break;
  default:
    printf("Unrecognized type: %d\n", node->type);
  }

  if (!IsLeaf(node))
  {
    if (node->left != nullptr)
      _print_tree(node->left);
    else
      fputc('@', tr);
    if (node->right != nullptr)
      _print_tree(node->right);
    else
      fputc('@', tr);
  }

  fprintf(tr, "}");


#undef IsLeaf
  return true;
} /* End of '_print_tree' function */

/**
 * \brief Get symbol operator tree color function
 * \param [in] num  operator num.
 * \return pointer to color string.
 */ 
const char * ad6::base_translator::_get_op_col( char num ) const
{
  unsigned size = sizeof(smb_op) / sizeof(oper);

  for (unsigned i = 0; i < size; i++)
    if (smb_op[i].get_num() == num)
      return smb_op[i].get_color();

  TREE_ASSERT(0, "Incorrect symbol operator");
  return nullptr;
} /* End of '_get_op_col' function */

/**
 * \brief Get string operator tree shape function
 * \param [in] name  reference to operator name.
 * \return pointer to shape string.
 */ 
const char * ad6::base_translator::_get_op_shp( const string &name ) const
{
  unsigned size = sizeof(pol_op) / sizeof(oper);

  for (unsigned i = 0; i < size; i++)
    if (StrChrCmp(( char *)pol_op[i].get_name(), name) == 0)
      return pol_op[i].get_shape();

  TREE_ASSERT(0, "Incorrect string operator");
  return nullptr;
} /* End of '_get_op_shp' function */

// class destructor
ad6::base_translator::~base_translator( void )
{
  if (root != nullptr)
    delete root;
  if (buf != nullptr)
    delete[] buf;
}

/**
 * \brief Read tree from file function
 * \param [in] filename Name of a file to read from.
 */
void ad6::base_translator::_read_tree( const char filename[] )
{
  TREE_ASSERT(filename != nullptr, "Incorrect file name");

  buf = FillBuf(filename, &buf_size);

  TREE_ASSERT(buf != nullptr, "Error with opening file");
    
  //  root = new node;
  
  buf_ptr = buf;

  _build_tree(&root);
} /* End of 'read_tree' function */

/**
 * \brief Build tree from file function
 * \param [in] root Pointer to tree root;
 */
void ad6::base_translator::_build_tree( node **nd )
{
#define SWTCH(nde, adds)                             \
  switch (*buf_ptr)                                  \
  {                                                  \
  case '@':                                          \
    (*nd)->##nde = nullptr;                          \
    buf_ptr++;                                       \
    break;                                           \
  case '{':                                          \
     adds;                                           \
    _build_tree(&((*nd)->##nde));                    \
    break;                                           \
  case '}':                                          \
    buf_ptr++;                                       \
    return;                                          \
  }

  //TREE_ASSERT(nd != nullptr, "node was nullptr");
  
  unsigned tok_size = 0;

  sscanf(buf_ptr, "{%*[^@{}]%n", &tok_size);
  tok_size--;
  buf_ptr++;
  TREE_ASSERT(tok_size != 0, "Incorrect tree file format");

  if (isdigit((unsigned char)*buf_ptr) || isdigit((unsigned char)buf_ptr[1]))
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

  SWTCH(left, 
    {
      if ((*nd)->type == TYPE_SEP && (*nd)->name[0] == ';')
      {
        if (buf_ptr[1] == '=')
          act_fnc = GLOBAL_VAR;
      }
      else if ((*nd)->type == TYPE_USR_FNC)
      {
        if (funcs.find(fnc((*nd)->name)) == -1)
          act_fnc = funcs.size();
      }
    });
  SWTCH(right,
    {
      if ((*nd)->type == TYPE_USR_FNC )
      {
        act_fnc = funcs.size();
        (*nd)->num = act_fnc;
        funcs.add(fnc((*nd)->name, _cnt_args(*nd)));
      }
    });
  buf_ptr++;

#undef SWTCH
} /* End of '_build_tree' function */


int ad6::base_translator::_cnt_args( node *nd )
{
  int cnt = 0;
  if (nd->left != nullptr)
  {
    cnt++;
    cnt += _cnt_args(nd->left);
  }
  return cnt;
}

/**
 * \brief Check token in buf_ptr and create a node function.
 * \param [in] tok_size  token name size
 * \return reference to new node.
 */
ad6::node * ad6::base_translator::_check_buf_ptr( unsigned tok_size )
{
  node *res = new node;
  string name(buf_ptr, tok_size);
  if (tok_size == 1)
  {
    node_type tpe = _check_one_smb(*buf_ptr);
    
    int num = 0;
    if (tpe == TYPE_VAR)
    {
      var _new(name, act_fnc);
      num = vars.find(_new);

      if (num == -1)
      {
        vars.add(_new);
        num = vars.size() - 1;
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
    var _new(name, act_fnc);
    num = vars.find(_new);

    if (num == -1)
    {
      vars.add(_new);
      num = vars.size() - 1;
    }
    res->set_node(TYPE_VAR, name, num);
    return res;
  }
   
  res->set_node(tpe, name, 0);
  return res;
} /* End of '_check_buf_ptr' function */

/**
 * \brief Recognize type of a branch function.
 * \param [in] str  pointer to string with a branch.
 * \param [in] size size of string.
 * \return type of a branch.
 */
ad6::node_type ad6::base_translator::_check_ops( const  char *str, unsigned size )
{
#define STRCMP(str, type)                   \
  if (ad6::StrChrCmp(#str, opr) == 0)       \
     return ad6::##type;

  unsigned ops_size = sizeof(ad6::pol_op) / sizeof(ad6::oper);
  ad6::string opr(str, size);

  for (unsigned i = 0; i < ops_size; i++)
    if (ad6::StrChrCmp(ad6::pol_op[i].get_name(), opr) == 0)
      return ad6::TYPE_POL_OP;

  STRCMP(>=, TYPE_CMP);
  STRCMP(<=, TYPE_CMP);
  STRCMP(!=, TYPE_CMP);
  STRCMP(==, TYPE_CMP);

  STRCMP(op, TYPE_SEP);

#define DEF_FNC(name, num, code, code2, code3)               \
  else if (ad6::StrChrCmp(#name, opr) == 0)           \
    return ad6::TYPE_FUNC;

  if (0);

#include "..\func.h"


#undef DEF_FNC

#undef STRCMP
  return TYPE_VAR;
} /* End of '_check_ops' function */

/**
 * \brief Check one symbol type function.
 * \param smb  Symbol to check.
 * \return type of a symbol.
 */
ad6::node_type ad6::base_translator::_check_one_smb( char smb )
{
  unsigned size = sizeof(smb_op) / sizeof(oper);

  for (unsigned i = 0; i < size; i++)
    if (smb == smb_op[i].get_num())
      return TYPE_OPERATOR;
  if (smb == '=')
    return TYPE_OPERATOR;

  if (smb == ';' || smb == ',')
    return TYPE_SEP;
  if (smb == '>' || smb == '<')
    return TYPE_CMP;
  //TREE_ASSERT(isalpha((unsigned char)smb), "Unrecognized symbol");
 
  return TYPE_VAR;
} /* End of '_check_one_smb' function */

/**
 * \brief Save tree to file function.
 * \param [in]  filename  Name of a file to save.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::base_translator::_save_tree( const char filename[] )
{
  TREE_ASSERT(filename != nullptr, "Incorrect file name");

  tr = fopen(filename, "w");

  if (tr == nullptr)
  {
    printf("Oh, some errors with openning file \"%s\"", filename);
    return false;
  }

  _print_tree(root);

  fclose(tr);
  return true;
} /* End of '_save_tree' function */