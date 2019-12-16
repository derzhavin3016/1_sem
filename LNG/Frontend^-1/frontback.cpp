#include "frontback.h"

// class constructor
ad6::fback::fback( void ) : tree()
{
}

/**
 * \brief Convert tree to code function.
 * \param [in]  filename  name of a file to save code.
 */
void ad6::fback::_tree_to_txt( const char filename[] )
{
  FB_ASSERT(filename != nullptr, "Incorrect filename");

  tr = fopen(filename, "wb");

  FB_ASSERT(tr != nullptr, "Error with opening file");

  variables.clear();

  _rec_print(root, 0);

  fclose(tr);
} /* End of '_tree_to_txt' fucntion */

/**
 * \brief Recursive print tree to opened file function.
 * \param [in] nd                      pointer to tree's node.
 * \param [in] ind_size                indent size.
 */
void ad6::fback::_rec_print( node *nd, int ind_size )
{
  FB_ASSERT(tr != nullptr, "File wasn't open");
  FB_ASSERT(nd != nullptr, "Node was nullptr");

#define L left

#define R right

#define PRINTF(str) fprintf(tr, str)

#define PRINT(nde, size)  _rec_print(nd->##nde, size)

#define PRINT_R(size) PRINT(R, size)

#define PRINT_L(size) PRINT(L, size)

#define SPC_DEF _print_spaces(ind_size)

  switch (nd->type)
  {
  case TYPE_CMP:
    PRINT_L(0);
    _print_spaces();
    nd->name.print_in_file(tr);
    _print_spaces();
    PRINT_R(0);
    break;
  case TYPE_FUNC:
    nd->name.print_in_file(tr);
    fprintf(tr, "(");
    if (nd->left != nullptr)
      PRINT_L(0);
    fprintf(tr, ")");
    break;
  case TYPE_NUMBER:
    fprintf(tr, "%lg", nd->value);
    break;
  case TYPE_OPERATOR:
    if (nd->num == '=')
    {
      if (!_check_add_var(nd->left->name))
        fprintf(tr, "var ");
      PRINT_L(0);
      fprintf(tr, " = ");
      PRINT_R(0);
    }
    else
    {
      PRINT_L(0);
      fprintf(tr, " %c ", nd->num);
      PRINT_R(0);
    }
    break;
  case TYPE_POL_OP:
    _print_pol_op(nd, ind_size);
    break;
  case TYPE_SEP:
    _print_sep(nd, ind_size);
    break;
  case TYPE_USR_FNC:
    variables.clear();
    if (nd->right != nullptr)
      PRINTF("$");
    nd->name.print_in_file(tr);


    fprintf(tr, "(");
    if (nd->left != nullptr)
      PRINT_L(0);
    fprintf(tr, ")");


    if (nd->right != nullptr)
    {
      fprintf(tr, "\n");
      SPC_DEF;
      fprintf(tr, "{\n");
      PRINT_R(ind_size + 2);
      SPC_DEF;
      fprintf(tr, "}\n");
    }
    break;
  case TYPE_VAR:
    nd->name.print_in_file(tr);
    break;
  default:
    std::cerr << "Unrecognized type" << nd->type << std::endl;
    throw Error("", FB_LOCATION);
  }

#undef L

#undef R

#undef PRINT

#undef PRINT_R

#undef PRINT_L

#undef SPC_DEF

#undef PRINTF
} /* End of '_rec_print' function */

/**
 * \brief Print polysymbols operators in opened file function.
 * \param [in] nd                      pointer to tree's node.
 * \param [in] ind_size                indent size.
 */
void ad6::fback::_print_pol_op( node *nd, int ind_size )
{
  FB_ASSERT(tr != nullptr, "File wasn't open");
  FB_ASSERT(nd != nullptr, "Node was nullptr");
  FB_ASSERT(nd->type == TYPE_POL_OP, "Incorrect type, expected 'TYPE_POL_OP'");


#define PRINT(str) fprintf(tr, str)

#define SPC_DEF _print_spaces(ind_size)

#define CMP_OP(str)  (StrChrCmp(str, nd->name) == 0)

  if (CMP_OP("if"))
  {
    PRINT("if");
    if (nd->left != nullptr) 
    {
      _print_spaces();
      PRINT("(");
      _rec_print(nd->left, 0);
      PRINT(")\n");
    }
    if (nd->right != nullptr) 
    {
      SPC_DEF;
      PRINT("{\n");
      _rec_print(nd->right->left, ind_size + 2);
      SPC_DEF;
      PRINT("}\n");
      if (nd->right->right != nullptr)
      {
        SPC_DEF;
        PRINT("else\n");
        SPC_DEF;
        PRINT("{\n");
        _rec_print(nd->right->right, ind_size + 2);
        SPC_DEF;
        PRINT("}\n");
      }
    }
  }
  else if (CMP_OP("ret"))
  {
    PRINT("return");
    if (nd->left != nullptr) 
    {
      _print_spaces();
      _rec_print(nd->left, 0);
    }
    PRINT(";\n");
  }
  else if (CMP_OP("while"))
  {
    PRINT("while");
    if (nd->left != nullptr) 
    {
      _print_spaces();
      PRINT("(");
      _rec_print(nd->left, 0);
      PRINT(")\n");
    }
    if (nd->right != nullptr)
    {
      SPC_DEF;
      PRINT("{\n");
      _rec_print(nd->right, ind_size + 2);
      SPC_DEF;
      PRINT("}\n");
    }
  }
  else
  {
    std::cerr << "Unrecognized operator :" << nd->name << std::endl;
    throw Error("", FB_LOCATION);
  }

#undef CMP_OP

#undef SPC_DEF

#undef PRINT
} /* End of '_print_pol_op' function */

/**
 * \brief Print separations operators in opened file function.
 * \param [in] nd                      pointer to tree's node.
 * \param [in] ind_size                indent size.
 */
void ad6::fback::_print_sep( node *nd, int ind_size )
{
  FB_ASSERT(tr != nullptr, "File wasn't open");
  FB_ASSERT(nd != nullptr, "Node was nullptr");
  FB_ASSERT(nd->type == TYPE_SEP, "Incorrect type, expected 'TYPE_SEP'");

  #define PRINT(str) fprintf(tr, str)

  #define SPC_DEF _print_spaces(ind_size)

  SPC_DEF;

#define STRCMP(str) (StrChrCmp(str, nd->name) == 0)

  if (STRCMP(","))
  {
    if (nd->right != nullptr)
      _rec_print(nd->right, 0);
    if (nd->left != nullptr)
    {
      PRINT(", ");
      _rec_print(nd->left, 0);
    }
  }
  else if (STRCMP(";"))
  {
    if (nd->left != nullptr)
    {
      _rec_print(nd->left, 0);
    }
    if (nd->right != nullptr)
    {
      _rec_print(nd->right, 0);
    }
  }
  else if (STRCMP("op"))
  {
    if (nd->left != nullptr)
    {
      _rec_print(nd->left, ind_size);
      if (nd->left->type != TYPE_POL_OP)
        PRINT(";\n");
    }
    if (nd->right != nullptr)
      _rec_print(nd->right, ind_size);
  }

#undef PRINT

#undef STRCMP

#undef SPC_DEF

} /* End of '_print_sep' function */

/**
 * \brief Print spaces to opened file function.
 * \param [in]  spc_amount     amount of spaces (1 default)
 */
void ad6::fback::_print_spaces( int spc_amount /*= 1*/ )
{
  for (int i = 0; i < spc_amount; i++)
    fprintf(tr, " ");
} /* End of '_print_spaces' fucntion */

/**
 * \brief Find variable in variables array, or add it.
 * \param [in]    name              reference to name string.
 * \return true if variable was find
 * \return false if variable wasn't find.
 */
bool ad6::fback::_check_add_var( string &name )
{
  int num = variables.find(name);

  if (num == -1)
  {
    variables.add(name);
    return false;
  }
  return true;
} /* End of '_check_add_var' function */

// class destructor
ad6::fback::~fback( void )
{
}

