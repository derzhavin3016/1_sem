#include "mid.h"



/**
 * \brief Create differntiate tree function
 */
bool ad6::mid::tree_diff( node **start )
{
  TREE_ASSERT(*start != nullptr, "Root was nullptr");

  if ((*start)->type == TYPE_POL_OP && StrChrCmp("deriv", (*start)->name) == 0)
  {
    diff = &rec_diff(*((*start)->right), (*start)->left->num);
    _simplifier(&diff);
    delete *start;
    *start = diff;
    diff = nullptr;
  }
  else
  {
    if ((*start)->left != nullptr)
      tree_diff(&(*start)->left);
    if ((*start)->right != nullptr)
      tree_diff(&(*start)->right);
  }
  
  return true;
} /* End of 'tree_diff' function */

/**
 * \brief Recursive differentiator of tree. 
 */
ad6::node & ad6::mid::rec_diff( node &nd, size_t var_num )
{
  #define dL rec_diff(*(nd.left), var_num)
  #define dR rec_diff(*(nd.right), var_num)
  
  #define cL *(rec_copy(nd.left))
  #define cR *(rec_copy(nd.right))


  switch (nd.type)
  {
  case TYPE_OPERATOR:

  #define DEF_OP(num, name, calc, diff)              \
    case num:                                        \
      diff;                                          \
      break;
 
    switch (nd.num)
    {

  #include "..\cmd.h"

    default:
      TREE_ASSERT(0, "Unrecognized type");
    }
#undef DEF_OP

    break;
  case TYPE_NUMBER:
    return *(new node(0));
    break;
  case TYPE_VAR:
    if (var_num == nd.num)
      return *(new node(1));
    return *(new node(0));
    break;
  case TYPE_FUNC:

  #define DEF_FNC(name, num ,diff, calc)                     \
      case num:                                        \
        diff;                                          \
        break;

    switch (nd.num)
    {
 
    #include "..\..\Differentiator\func.h"

    default:
      TREE_ASSERT(0, "Unrecognized function");
    }

  #undef DEF_FNC
    break;
  case TYPE_POL_OP:
    if (StrChrCmp("deriv", nd.name) == 0)
      return rec_diff(*(nd.right), nd.left->num);
    else
      TREE_ASSERT(0, "Incorrect expression");
    break;
  default:
    TREE_ASSERT(0, "Unrecognized type");
    break;
  }
  return nd;

  #undef dL
  #undef dR
  #undef cL
  #undef cR
} /* End of 'rec_diff' function */

/**
 * \brief Recursive calculating tree function
 * \param nd  Pointer to tree's node.
 * \return Calculated value.
 */
double ad6::mid::rec_calc( node *nd ) const
{
  TREE_ASSERT(nd != nullptr, "root was nullptr");
  
#define DEF_OP(num, name, calc, diff)  \
    case num:                    \
      calc;

#define DEF_FNC(name, num, diff, calc)  \
    case num:                           \
      calc;

  switch (nd->type)
  {
  case TYPE_NUMBER:
    return nd->value;
  case TYPE_OPERATOR:
     
    switch (nd->num)
    {
      #include "..\cmd.h"
    default:
      printf("Incorrect operator num : %d\n", nd->num);
    }
    break;
  case TYPE_FUNC:
    switch (nd->num)
    {
     
    #include "..\..\Differentiator\func.h"

    default:
      printf("Incorrect function num : %d\n", nd->num);
    }
    break;
  default:
    printf("Unrecognized type %d\n", nd->type);
    return 0;
  }
  #undef DEF_OP
  
  #undef DEF_FNC

  return 0;
} /* End of 'rec_calc' function */

/**
 * \brief Simplify tree function.
 * \param [in] nd pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
*/
bool ad6::mid::_simplifier( node **nd )
{
  symp_counter = 0;
  int prev_cnt = 1;

  *nd = _rec_symp(*nd);
  
  while (prev_cnt != symp_counter)
  {
    prev_cnt = symp_counter;
    *nd = _rec_symp(*nd);
  }

  return true;
} /* End of '_simplifier' function */

/**
 * \brief Calculate tree if it is possible.
 * \param [in] pointer to node.
*/
bool ad6::mid::_calc_tree( node *nd )
{
  TREE_ASSERT(nd != nullptr, "Node was nullptr");

  if (_is_calc(nd) && nd->type != TYPE_NUMBER)
  {
    nd->value = rec_calc(nd);
    nd->type = TYPE_NUMBER;
    if (nd->right)
      delete nd->right;
    if (nd->left)
      delete nd->left;
    nd->right = nullptr;
    nd->left = nullptr;
    symp_counter++;
    return true;
  }
  return false;
} /* End of '_calc_tree' function */

/**
 * \brief Recursive simplifier function
 * \param [in] nd pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
*/
ad6::node* ad6::mid::_rec_symp( node *nd )
{
  TREE_ASSERT(nd != nullptr, "Node was nullptr");

  if (_calc_tree(nd))
    return nd;


  if (nd->left != nullptr)
    nd->left = _rec_symp(nd->left);

  if (nd->right != nullptr)
    nd->right = _rec_symp(nd->right);
 
  switch (nd->type)
  {
  case TYPE_NUMBER:
    return nd;
  case TYPE_VAR:
    return nd;
  case TYPE_FUNC:
    if (nd->right != nullptr)
      nd->right = _rec_symp(nd->right);
    if (nd->left != nullptr)
      nd->left = _rec_symp(nd->left);
    return nd;
  case TYPE_OPERATOR:
    
#define DEF_SMP(num, simp)                               \
     case num:                                           \
      simp;                                              \
      break;

    switch (nd->num)
    {
    
    #include "..\..\Differentiator\simp.h"

    case '=':
      nd->right = _rec_symp(nd->right);
      return nd;
      break;
    default:
      TREE_ASSERT(0, "Unrecognized operator");
    }


    #undef DEF_SMP
    
    return nd;
    break;
  default:
    TREE_ASSERT(0, "Unrecognized type");
    return nullptr;
  }

  return nd;
} /* End of '_rec_symp' fucntion */

/**
 * \brief Checking if brabch is calculatable.
 * \param [in] nd pointer to tree.
*/
bool ad6::mid::_is_calc( node *nd ) const
{
  if (nd->type == TYPE_VAR || nd->type == TYPE_USR_FNC || nd->type == TYPE_POL_OP)
    return false;

#define CHECK_NODE(nd_check)                         \
  bool is_calc_##nd_check = true;                    \
  if (nd->##nd_check != nullptr)                     \
    is_calc_##nd_check = _is_calc(nd->##nd_check);

  CHECK_NODE(left);

  CHECK_NODE(right);


#undef CHECK_NODE
  return is_calc_left && is_calc_right;
} /* End of '_is_calc' function */


/**
 * \brief Find variable in tree function.
 * \param [in] start Pointer to tree's node.
 * \param [in] var_num Number of variable in variables array.
*/
bool ad6::mid::_find_var_tree( node *start, size_t var_num ) const
{
  if ((start->type == TYPE_VAR && start->num == var_num) || start->type == TYPE_USR_FNC)
    return false;
  
  #define CHECK_NODE(nd_check)                                      \
  bool is_var_##nd_check = true;                                    \
  if (start->##nd_check != nullptr)                                 \
    is_var_##nd_check = _find_var_tree(start->##nd_check, var_num);

  CHECK_NODE(left);

  CHECK_NODE(right);

#undef CHECK_NODE

  return is_var_left && is_var_right;
} /* End of '_find_var_tree' function */


void ad6::mid::translate( const char file_in[], const char file_out[] )
{
  _read_tree(file_in);
  tree_diff(&root);
  _save_tree(file_out);
}
