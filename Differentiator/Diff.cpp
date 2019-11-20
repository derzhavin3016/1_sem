#include "Diff.h"

#define GREETING  "Hello. I'm an artificial intelligence.\n"                              \
                  "My dad is Andrey Derzhavin, and he has his own group in vk\n"          \
                  "Subscribe, if you want to know more about my dad's phystech life\n"    \
                  "https://vk.com/andryha_mipt \n\n"                                      \
                  "*** Here what can I do: \n"                                            \
                  "*** 0. Exit (bye bye)\n"                                               \
                  "*** 1. Play \n"                                                        \
                  "*** 2. Save database to file\n"                                        \
                  "*** 3. Load database from file\n"                                      \
                  "*** 4. Dump tree with dot\n"                                           \
                  "*** 5. Print characteristics of the object\n"                          \
                  "*** 6. Compare two objects\n"

#define PARTING  "Have a nice night, stranger!\n"

/**
 * \brief Craete right and left leaves function function.
 */
bool ad6::tree::create_leaves( node *prnt )
{
  prnt->left = new node;
  prnt->right = new node;

  return true;
} /* End of 'Createnode' function */

/**
 * \brief Get user's answer from stdout function.
 * \brief This function put scanned string to static buffer, allocate memory for the answer and return pointer.
 * \param printfstr   String for printf.
 * \param  ...        Printf parameters.
 * \return  pointer to the answer.
 * \warning Using static buffer.
 */
char * ad6::InputAnswer( const char printfstr[], ... )
{
  static char buf[ad6::ANSWER_MAX] = {};
  va_list args;

  if (printfstr == nullptr)
    return buf;

  va_start(args, printfstr);
  vprintf(printfstr, args);
  va_end(args);

  scanf("\n%1024[^\n]", buf);

  return buf;
} /* End of 'InputAnswer' function */

/**
 * \brief Process loop function.
 */
void ad6::tree::process_loop( void )
{
  printf(GREETING);
  printf("\n\n");
  int promt = 0;

  while (1)
  {
    printf("Input number to start:");
    scanf("%d", &promt);
    switch (promt)
    {
    case 0:
      printf(PARTING);
      return;
    case 1:
      break;
    case 2:
      save_tree(InputAnswer("Input file name to save tree:\n"));
      break;
    case 3:
      read_tree(InputAnswer("Input file name to load tree from:\n"));
      break;
    case 4:
      dump(InputAnswer("Input file name to dump tree (only name):\n"));
      printf("Dump success\n");
      break;
    default:
      printf("Incorrect number of command: %d\n", promt);
    }
  }
}

/**
 * \brief Save tree to file function.
 * \param [in]  filename  Name of a file to save.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::tree::save_tree( const char filename[] ) const
{
  TREE_ASSERT(filename != nullptr, "Incorrect file name");

  FILE *tree = fopen(filename, "w");

  if (tree == nullptr)
  {
    printf("Oh, some errors with openning file \"%s\"", filename);
    return false;
  }

  print_tree(tree, root);

  fclose(tree);
  return true;
} /* End of 'save_tree' function */


/**
 * \brief Print tree to opened file function.
 * \param [in] node pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::tree::print_tree( FILE *f, node *node ) const
{
  TREE_ASSERT(f != nullptr, "File not opened\n");
  TREE_ASSERT(node != nullptr, "node was nullptr");

#define IsLeaf(node)  ((node)->right == nullptr && (node)->left == nullptr)

  fprintf(f, "{\"%s%c\"", node->name, IsLeaf(node) ? '!' : '?');
  if (node->right != nullptr)
    print_tree(f, node->right);
  if (node->left != nullptr)
    print_tree(f, node->left);
  
  fprintf(f, "}");


#undef IsLeaf
  return true;
} /* End of 'print_tree' function */

/**
 * \brief Read tree from file function.
 * \param [in] pointer to file structure
 * \param [in]  filename  Name of a file to read from.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::tree::read_tree( const char filename[] )
{
  TREE_ASSERT(filename != nullptr, "Incorrect file name");

  size_t buf_size = 0;
  char *buf = FillBuf(filename, &buf_size);

  char IsOk = 0;
  size_t pos = 0;
  TREE_ASSERT(buf != nullptr, "Error with opening file")

  if (root != nullptr)
    delete root;

  root = new node;

  build_tree(buf);

  delete[] buf;
  return true;
} /* End of 'read_tree' function */


/**
 * \brief Build tree from buffer function.
 * \param [in] pointer to buffer
 * \param [in] node pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::tree::build_tree( char *buf )
{
  TREE_ASSERT(buf != nullptr, "File not opened");
 

#define CHECK_BR if (((new_ptr = SkipSpaces(buf_ptr)) == nullptr) || *new_ptr != ')')                                  \
                 {                                                                                                     \
                   char buffer[ANSWER_MAX] = {0};                                                                      \
                   sprintf(buffer, "Expected ')' in position %d, but met '%c'", buf_ptr - buf + 1, *new_ptr);          \
                                                                                                                       \
                   throw Error(buffer, TREE_LOCATION);                                                                 \
                 }
 
  char *buf_ptr = buf;
  node **cur = &root;
  size_t pos = 0;

  while (*buf_ptr != 0)
  {
    char *new_ptr = nullptr;
    if ((new_ptr = SkipSpaces(buf_ptr)) == nullptr)
      break;
    buf_ptr = new_ptr;

    if (*buf_ptr == '(')
    {
      (*cur)->left = new node;
      (*cur)->left->parent = *cur;
      buf_ptr++;
      cur = &(*cur)->left;
      continue;
    }
    else if (sscanf(buf_ptr, " %lg%n", &(*cur)->value, &pos) == 1)
    {
      buf_ptr += pos;
      (*cur)->type = TYPE_NUMBER;
      CHECK_BR;
      buf_ptr = new_ptr;
      cur = &(*cur)->parent;  
      continue;
    }
    else if (*buf_ptr != ')')
    {
      (*cur)->name = new char[ANSWER_MAX];
      sscanf(buf_ptr, " %[^ ()]%n", (*cur)->name, &pos);
      buf_ptr += pos;
      buf_ptr = SkipSpaces(buf_ptr);

      size_t number = 0;
      if ((number = find_op((*cur)->name)) != -1)
      {
        buf_ptr++;
        (*cur)->type = TYPE_OPERATOR;
        (*cur)->num = number;
        
        (*cur)->right = new node;
        (*cur)->right->parent = (*cur);

        cur = &(*cur)->right;

        continue;
      }
      else
      {
        (*cur)->type = TYPE_VAR;
        int num = 0;

        if ((num = variables.FindValue((*cur)->name, StrCompare) == -1))
          num = variables.Push_head((*cur)->name);
        (*cur)->num = (size_t)num;
        
        CHECK_BR;

        buf_ptr = new_ptr + 1;
        cur = &(*cur)->parent;  
        continue;
      }
    }
    else
    {
      if ((*cur)->parent == nullptr)
      {
        char buffer[ANSWER_MAX] = {0};
        sprintf(buffer, "Detected exess ')' in position %d\n", buf_ptr - buf + 1);
        
        throw Error(buffer, TREE_LOCATION);
      }

      buf_ptr++;

      cur = &(*cur)->parent;
      continue;
    }
  }

  char buffer[ANSWER_MAX] = {0};
  sprintf(buffer, "Expected ')' in position %d, but met the end of the line", buf_ptr - buf + 1);

#undef CHECK_BR

  TREE_ASSERT(*cur == root, buffer);


  return true;
} /* End of 'build_tree' function */

/**
 * \brief Find operator number function.
 * \brief string ppointer to string.
 * \return number of the operator if it was find.
 * \return -1 otherwise.
*/
int ad6::tree::find_op( char *string )
{
  TREE_ASSERT(string != nullptr, "string was nullptr");

  #define DEF_OP(num, name, calc, str)                 \
     if (stricmp(str, string) == 0)            \
       return num;

  #include "cmd.h"

  #undef DEF_OP

  return -1;
} /* End of 'find_op' function */


/**
 * \brief treenator dump function.
 * \param filename Name of a file to dump.
 */
bool  ad6::tree::dump( const char filename[] ) const
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

  if (root->left == nullptr && root->right == nullptr)
    fprintf(dmp, "\"%s\"", root->name);
  else
    rec_dump(dmp, root);

  fprintf(dmp, "}");

  fclose(dmp);
 
  sprintf(buf, "dot -Tpng %s.dot -o%s.png", filename, filename);

  system(buf);
  return true;
} /* End of 'dump' funciton */

/**
 * \brief Recursion dump tree function.
 * \param dmp  Pointer to opened file structure.
 * \param node  Pointer to node.
 */
void ad6::tree::rec_dump( FILE *dmp, node *node ) const
{ 
  TREE_ASSERT(dmp != nullptr, "File not opened");
  TREE_ASSERT(node != nullptr, "node was nullptr");

  switch (node->type)
  {
  case TYPE_OPERATOR:
    if (node->left != nullptr)
    {
      fprintf(dmp, "\"%p\" -> ", node);
      rec_dump(dmp, node->left);
      fprintf(dmp, "\"%p\" [label = \"%s\"];\n", node, node->name);
    }
    if (node->right != nullptr)
    {
      fprintf(dmp, "\"%p\" -> ", node);
      rec_dump(dmp, node->right);
      fprintf(dmp, "\"%p\" [label = \"%s\"];\n", node, node->name);
    }
    break;
  case TYPE_NUMBER:
    fprintf(dmp, "\"%p\";\n\"%p\" [label = \"%lg\"];\n", node, node, node->value);
    break;
  case TYPE_VAR:
    fprintf(dmp, "\"%p\";\n\"%p\" [label = \"%s\"];\n", node, node, node->name);
    break;
  default:
    printf("Unrecognised type : %d", node->type);
    break;
  }
#undef IsLeaf
} /* End of 'rec_dump' function */


/**
 * \brief Caclulating tree's expression.
 * \param start  Pointer to tree's root;
 * \return Calculated value.
 */
double ad6::tree::tree_calc( void ) const
{
  TREE_ASSERT(root != nullptr, "root was nullptr");

  
  return rec_calc(root);
} /* End of 'tree_calc' function */

void ad6::tree::tree_diff( void )
{
  TREE_ASSERT(root != nullptr, "Root was nullptr");
  
  *diff = rec_diff(*root);
} /* End of 'tree_diff' function */

ad6::node & ad6::tree::rec_diff( node &nd )
{
  switch (nd.type)
  {
    
  }
  return nd;
} /* End of 'rec_diff' function */

/**
 * \brief Recursive calculating tree function
 * \param nd  Pointer to tree's node.
 * \return Calculated value.
 */
double ad6::tree::rec_calc( node *nd ) const
{
  TREE_ASSERT(nd != nullptr, "root was nullptr");
  
#define DEF_OP(num, name, calc, str)  \
    case num:                    \
      calc;

  switch (nd->type)
  {
  case TYPE_NUMBER:
    return nd->value;
  case TYPE_OPERATOR:
     
    switch (nd->num)
    {
      #include "cmd.h"
    default:
      printf("Incorrect operator num : %d\n", nd->num);
    }
    break;
  default:
    printf("Unrecognized type %d\n", nd->type);
    return 0;
  }

  return 0;
#undef DEF_OP
} /* End of 'rec_calc' function */

