#include "Diff.h"

#define TREE_LOCATION  __LINE__, __FILE__, __FUNCSIG__

#define TREE_ASSERT(cond, err)           if (!(cond))                               \
                                        {                                          \
                                          throw Error{err, TREE_LOCATION};          \
                                        }

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
  buf = DelSpaces(buf, buf_size, true);

  char IsOk = 0;
  size_t pos = 0;
  TREE_ASSERT(buf != nullptr, "Error with opening file")

  base.Kill();
  if (root != nullptr)
    delete root;

  root = new node;

  char *buf_ptr = buf + pos;
  build_tree(&buf_ptr, root);

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
bool ad6::tree::build_tree( char **buf, node *nd )
{
  TREE_ASSERT(buf != nullptr, "File not opened");
  TREE_ASSERT(nd != nullptr, "Node was nullptr");
  
  size_t pos = 0;

  if (**buf == 0)
    return true;

  if (**buf == '(')
  {
    nd->left = new node;
    nd->left->parent = nd;
    (*buf)++;
    build_tree(buf, nd->left);
  }
  else if (sscanf(*buf, " %lg%n", &nd->value, &pos) == 1)
  {
    *buf += pos;
    nd->type = TYPE_NUMBER;
    (*buf)++;
    build_tree(buf, nd->parent);
  }
  else if (**buf != ')')
  {
    nd->type = TYPE_OPERATOR;
    nd->name = new char[ANSWER_MAX];
    sscanf(*buf, " %[^()]%n", nd->name, &pos);
    *buf += pos + 1;
    nd->right = new node;
    nd->right->parent = nd;
    build_tree(buf, nd->right);
  }
  else
  {
    (*buf)++;
    if (nd->parent != nullptr)
      build_tree(buf, nd->parent);
  }

  return true;
} /* End of 'build_tree' function */

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
  default:
    printf("Unrecognised type : %d", node->type);
    break;
  }


#undef IsLeaf

} /* End of 'rec_dump' function */


