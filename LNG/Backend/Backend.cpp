#include "Backend.h"

#define PUSH_BX(num)                                  \
{                                                     \
  _fprint("push %d\n", (num));                        \
  _fprint("push bx\n");                               \
  _fprint("add\n");                                   \
  _fprint("pop dx\n");                                \
  _fprint("push [dx]; ");                             \
  nd->name.print_in_file(tr);                         \
  _fprint("\n");                                      \
}

#define POP_BX(num, nde)                                  \
{                                                     \
  _fprint("push %d\n", (num));                        \
  _fprint("push bx\n");                               \
  _fprint("add\n");                                   \
  _fprint("pop dx\n");                                \
  _fprint("pop [dx]; ");                              \
   ##nde->name.print_in_file(tr);                     \
  _fprint("\n");                                      \
}

// class constructor
ad6::backend::backend( void ) : num_if(0),
                                num_while(0),
                                var_cnt(0)
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
  FillFncTables(funcs, vars);
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

  _fprint("push 0\n"
          "pop bx\n");
 
  _push_globals();
  _fprint("call main\n"
          "end\n\n");
  string _main("main", 4);

  act_fnc = _change_fnc(_main);

  _rec_print_asm(root);

  fclose(tr);
} /* End of '_to_asm' function */

void ad6::backend::_push_globals( void )
{
  int glbl_cnt = 0;
  node **nd = &root;

  for (unsigned i = 0; i < vars.size(); i++)
    if (vars[i].get_fnc_num() == GLOBAL_VAR)
    {
      vars[i].set_offset(glbl_cnt++);
      vars[i].Init();
    }
  bool IsDecl = false;

  while ((*nd)->right != nullptr)
  {
    if ((*nd)->left->type == TYPE_OPERATOR)
    {
      IsDecl = true;
      _rec_print_asm((*nd)->left->right);
      POP_BX(vars[(*nd)->left->num].get_offset(), (*nd));
    }
    nd = &((*nd)->right);
  }
  if (IsDecl)
    _fprint(" push dx\n pop bx\n");
}

void ad6::backend::_fprint( const char format[], ... )
{
  va_list args;

  va_start(args, format);
  vfprintf(tr, format, args);
  va_end(args);
} /* End of '_fprint' fucntion */

int ad6::backend::_change_fnc( string &name )
{
  int num = funcs.find(fnc(name));
  BE_ASSERT(num != -1, "Function wasn't found");

  return num;
}

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
    if (vars[nd->num].get_fnc_num() != GLOBAL_VAR)
      PUSH_BX(vars[nd->num].get_offset())
    else
      _fprint("push %d\npop dx\npush[dx]\n", vars[nd->num].get_offset());
    break;
  case TYPE_OPERATOR:
    _asm_op(nd);
    break;
  case TYPE_FUNC:
#define DEF_FNC(name_fn, num, code1, code2, code3)            \
   else if (StrChrCmp(#name_fn, nd->name) == 0)               \
   {                                                          \
     if (nd->right != nullptr)                                \
      _rec_print_asm(nd->right);                              \
     code3;                                                   \
   }

   if (0);

#include "..\func.h"


#undef DEF_FNC
    break;
  case TYPE_SEP:
    if (nd->name[0] != ',')
    {
      _rec_print_asm(nd->left);
      if (nd->right != nullptr)
        _rec_print_asm(nd->right);
    }
    else
    {
      _rec_print_asm(nd->right);
      if (nd->left != nullptr)
        _rec_print_asm(nd->left);
    }
    break;
  case TYPE_USR_FNC:
    _asm_fnc(nd);
    break;
  case TYPE_POL_OP:
    _asm_pol_op(nd);
    break;
  case TYPE_CMP:
    _asm_cmp(nd);
    break;
  default:
    break;
  }
} /* End of '_rec_print_asm' function */

void ad6::backend::_asm_cmp( node *nd )
{
#define STRCMP(str)  (StrChrCmp(str, nd->name) == 0)
  
  _rec_print_asm(nd->left);
  _rec_print_asm(nd->right);

  if (nd->name[0] == '>')
  {
    if (nd->name.size() == 2)
      _fprint("ja");
    else
      _fprint("jae");
  }
  else if (nd->name[0] == '<')
  {
    if (nd->name.size() == 2)
      _fprint("jb");
    else
      _fprint("jbe");
  }
  else if (STRCMP("!="))
    _fprint("je");
  else if (STRCMP("=="))
    _fprint("jne");

#undef STRCMP
}


void ad6::backend::_asm_fnc( node *nd )
{
  if (nd->right == nullptr)
  {
    /// function call 
    ////////////////////////
    int var_cnt = funcs[act_fnc].var_nums();

    int args = funcs[nd->num].get_args();
    // get expressions in arguments (in right order)
    if (nd->left != nullptr)
    {
      _rec_print_asm(nd->left);
      for (int i = 0; i < args; i++)
      {
        POP_BX(var_cnt + i, nd);
      }
    }
    _fprint("push bx\n");
    _fprint("push bx\npush %d\nadd\n", var_cnt + args);
    _fprint("pop bx\n");
    _fprint("call ");
    act_fnc = nd->num;
    nd->name.print_in_file(tr);
    _fprint("\n");
  }
  else
  {
    act_fnc = nd->num;
    // fucntion declaration
    nd->name.print_in_file(tr);
    _fprint(":\n");
    _rec_print_asm(nd->right);
  }
}

/**
 * \brief compile operator function.
 * \param [in]  nd  pointer to tree's node.
 */
void ad6::backend::_asm_op( node *nd )
{
  BE_ASSERT(nd->type == TYPE_OPERATOR, "Incorrect type, expected 'TYPE_OPERATOR'");

  if (nd->num == '=')
  {
    if (vars[nd->left->num].get_fnc_num() != GLOBAL_VAR)
    {
      int num = nd->left->num;
      _rec_print_asm(nd->right);
      POP_BX(vars[num].get_offset(), nd);
    }
    else
    {
      _fprint("push %d\n", vars[nd->left->num].get_offset());
      _fprint("pop dx\n push[dx]\n");
    }
    return;
  }

  _rec_print_asm(nd->right);
  _rec_print_asm(nd->left);

  switch (nd->num)
  {
  case '+':
    _fprint("add\n");
    break;
  case '-':
    _fprint("sub\n");
    break;
  case '*':
    _fprint("mul\n");
    break;
  case '/':
    _fprint("div\n");
    break;
  case '^':
    _fprint("pow\n");
    break;
  default:
    std::cerr << "Unrecognized operator " << nd->num << std::endl;
    throw Error("", BE_LOCATION);
    break;
  }

} /* End of '_asm_op' function */

/**
 * \brief Compile polysymbols operators function.
 * \param [in]          pointer to node.
 */
void ad6::backend::_asm_pol_op( node *nd )
{
#define STRCMP(str)  (StrChrCmp(str, nd->name) == 0)

  if (STRCMP("if"))
  {
    int num = num_if++;
    _rec_print_asm(nd->left);
    _fprint(" else_if_%d\n", num);
    _rec_print_asm(nd->right->left);
    _fprint("jmp end_if_%d\n", num);
    _fprint("else_if_%d:\n", num);

    if (nd->right->right != nullptr)
      _rec_print_asm(nd->right->right);
    _fprint("end_if_%d:\n", num);
  }
  else if (STRCMP("while"))
  {
    int num = num_while++;
    _fprint("while_%d:\n", num);
    _rec_print_asm(nd->left);
    _fprint(" end_while_%d\n", num);
    _rec_print_asm(nd->right);
    _fprint("jmp while_%d\n", num);
    _fprint("end_while_%d:\n", num);
  }
  else if (STRCMP("ret"))
  {
    
    if (nd->left != nullptr)
    {
      // return with argument
      _rec_print_asm(nd->left);
      _fprint("pop cx\npop bx\npush cx\n");
    }
    _fprint("ret\n");
  }

#undef STRCMP
} /* End of '_asm_pol_op' function */

// class distructor
ad6::backend::~backend( void )
{

}