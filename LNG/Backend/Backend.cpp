#include "Backend.h"

// class constructor
ad6::backend::backend( void )
{

}

/**
 * \brief Translate tree to asm code function.
 * \param [in]  file_in           name of a file to read from.
 * \param [in]  file_out          name of a file to write.
 */
void ad6::backend::tree_to_asm( const char file_in[], const char file_out[] )
{
  _read_tree(file_in);
  _to_asm(file_out);
} /* End of 'tree_to_asm' function */

/**
 * \brief Write asm code to file function.
 * \param [in]   filename        name of a file to write.
 */
void ad6::backend::_to_asm( const char filename[] )
{
  BE_ASSERT(filename != nullptr, "Incorrect filename");

  tr = fopen(filename, "wb");

  BE_ASSERT(tr != nullptr, "Error with file opening");

  _fprint("call main\nend.\n");
  _rec_print_asm(root);

  fclose(tr);
} /* End of '_to_asm' function */

void ad6::backend::_fprint( const char format[], ... )
{
  va_list args;

  va_start(args, format);
  vfprintf(tr, format, args);
  va_end(args);
} /* End of '_fprint' fucntion */

/**
 * \brief Recursive print tree to asm code file function
 * \param [in]  nd              pointer to tree's node.
 */
void ad6::backend::_rec_print_asm( node *nd )
{
  BE_ASSERT(nd != nullptr, "Node was nullptr");


  switch (nd->type)
  {
  case TYPE_NUMBER:
    _fprint("push %d\n", (int)nd->value);
    break;
  case TYPE_VAR:
    break;
  case TYPE_OPERATOR:
    break;
  case TYPE_FUNC:
    break;
  case TYPE_SEP:
    break;
  case TYPE_USR_FNC:
    break;
  case TYPE_CMP:
    break;
  case TYPE_POL_OP:
    break;
  default:
    break;
  }
} /* End of '_rec_print_asm' function */

// class distructor
ad6::backend::~backend( void )
{

}