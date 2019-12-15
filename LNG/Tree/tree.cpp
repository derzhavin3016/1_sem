#include "tree.h"

ad6::tree::tree( void ) : root(nullptr),
               buf(nullptr),
               buf_size()
{

}


/**
 * \brief tree dump function.
 * \param filename Name of a file to dump.
 */
bool ad6::tree::dump( const char filename[], node *nd ) const
{
  TREE_ASSERT(filename != nullptr, "Incorrect file name");

  char buf[ANSWER_MAX] = {};

  sprintf(buf, "%s.dot", filename);

  FILE *dmp = fopen(buf, "w");

  if (dmp == nullptr)
  {
    printf("Oh, some errors with openning file %s\n", buf);
    return false;
  }

  fprintf(dmp, "digraph %s \n{\n", filename);
  
  rec_dump(dmp, nd);
  
  fprintf(dmp, "}\n");
  

  fclose(dmp);
 
  sprintf(buf, "dot -Tpng %s.dot -o%s.png", filename, filename);

  system(buf);
  return true;
} /* End of 'dump' function */

/**
 * \brief Recursion dump tree function.
 * \param dmp  Pointer to opened file structure.
 * \param node  Pointer to node.
 */
void ad6::tree::rec_dump( FILE *dmp, node *node ) const
{
  TREE_ASSERT(dmp != nullptr, "File not opened");
  TREE_ASSERT(node != nullptr, "node was nullptr");
#define PUT_END(shp, fill, col)              \
  fprintf(dmp, "\", shape = %s, style = %s, fillcolor = %s, color = black];\n", shp, fill, col);

#define CHECK_L                                         \
  if (node->left != nullptr)                            \
  {                                                     \
    fprintf(dmp, "\"%p\" -> ", node);                   \
    rec_dump(dmp, node->left);                          \
  }

#define CHECK_R                                         \
  if (node->right != nullptr)                           \
  {                                                     \
    fprintf(dmp, "\"%p\" -> ", node);                   \
    rec_dump(dmp, node->right);                         \
  }

#define CHECK_L_R  \
  {                \
    CHECK_L;       \
    CHECK_R;       \
  }


  switch (node->type)
  {
  case TYPE_OPERATOR:
    CHECK_L_R;

    fprintf(dmp, "\n\"%p\" [label = \"%c", node, node->num);
    if (node->num == '=')
      fprintf(dmp, "\", shape = larrow];\n");
    else
      PUT_END("diamond", "filled", _get_op_col(node->num));


    break;
  case TYPE_NUMBER:

    fprintf(dmp, "\"%p\";\n\"%p\" [label = \"%lg", node, node, node->value);
    PUT_END("egg", "filled", "darkseagreen2");
    break;
  case TYPE_VAR:
    fprintf(dmp, "\"%p\";\n\"%p\" [label = \"", node, node);

    node->name.print_in_file(dmp);
    PUT_END("egg", "filled", "darkolivegreen2");
    break;
  case TYPE_FUNC:
    CHECK_L;

    fprintf(dmp, "\"%p\"", node);

    fprintf(dmp, "\n\"%p\" [label = \"", node);
    node->name.print_in_file(dmp);
    PUT_END("box", "filled", "bisque");
    break;
  case TYPE_SEP:
    CHECK_L_R;

    fprintf(dmp, "\n\"%p\" [label = \"", node);
    node->name.print_in_file(dmp);
    fprintf(dmp, "\"];\n");
    break;
  case TYPE_USR_FNC:
    CHECK_L_R;
    
    fprintf(dmp, "\"%p\"", node);

    fprintf(dmp, "\n\"%p\" [label = \"$", node);
    node->name.print_in_file(dmp);
    PUT_END("box", "filled", "bisque");
    break;
  case TYPE_POL_OP:
    CHECK_L_R;

    fprintf(dmp, "\"%p\"", node);

    fprintf(dmp, "\n\"%p\" [label = \"", node);
    node->name.print_in_file(dmp);
    PUT_END(_get_op_shp(node->name), "solid", "black");
    break;
  case TYPE_CMP:
    CHECK_L_R;

    fprintf(dmp, "\n\"%p\" [label = \"", node);
    node->name.print_in_file(dmp);
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
} /* End of 'rec_dump' function */ 


/**
 * \brief Print tree to opened file function.
 * \param [in] node pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::tree::_print_tree( FILE *f, node *node ) const
{
  TREE_ASSERT(f != nullptr, "File not opened\n");
  TREE_ASSERT(node != nullptr, "node was nullptr");

#define IsLeaf(node)  ((node)->right == nullptr && (node)->left == nullptr)

  fprintf(f, "{");
  switch (node->type)
  {
  case TYPE_NUMBER:
    fprintf(f, "%lg", node->value);
    break;
  case TYPE_FUNC:
    node->name.print_in_file(f);
    break;
  case TYPE_OPERATOR:
    fputc(node->num, f);
    break;
  case TYPE_SEP:
    node->name.print_in_file(f);
    break;
  case TYPE_VAR:
    node->name.print_in_file(f);
    break;
  case TYPE_USR_FNC:
    fputc('$', f);
    node->name.print_in_file(f);
    break;
  case TYPE_POL_OP:
    node->name.print_in_file(f);
    break;
  case TYPE_CMP:
    node->name.print_in_file(f);
    break;
  default:
    printf("Unrecognized type: %d\n", node->type);
  }

  if (!IsLeaf(node))
  {
    if (node->left != nullptr)
      _print_tree(f, node->left);
    else
      fputc('@', f);
    if (node->right != nullptr)
      _print_tree(f, node->right);
    else
      fputc('@', f);
  }

  fprintf(f, "}");


#undef IsLeaf
  return true;
} /* End of '_print_tree' function */

/**
 * \brief Get symbol operator tree color function
 * \param [in] num  operator num.
 * \return pointer to color string.
 */ 
const char * ad6::tree::_get_op_col( char num ) const
{
  size_t size = sizeof(smb_op) / sizeof(oper);

  for (size_t i = 0; i < size; i++)
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
const char * ad6::tree::_get_op_shp( const string &name ) const
{
  size_t size = sizeof(pol_op) / sizeof(oper);

  for (size_t i = 0; i < size; i++)
    if (StrChrCmp(pol_op[i].get_name(), name) == 0)
      return pol_op[i].get_shape();

  TREE_ASSERT(0, "Incorrect string operator");
  return nullptr;
} /* End of '_get_op_shp' function */

ad6::tree::~tree( void )
{
  if (root != nullptr)
    delete root;
  if (buf != nullptr)
    delete[] buf;
}