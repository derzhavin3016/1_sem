#include "Akinator.h"

#define GREETING  "Hello. I'm an artificial intelligence.\n"                              \
                  "My dad is Andrey Derzhavin, and he has his own group in vk\n"          \
                  "Subscribe, if you want to know more about my dad's phystech life\n"    \
                  "https://vk.com/andryha_mipt \n\n"                                      \
                  "*** Here what can I do: \n"                                            \
                  "*** 0. Exit (bye bye)\n"                                             \
                  "*** 1. Play \n"                                                        \
                  "*** 2. Save database to file\n"                                        \
                  "*** 3. Load database from file\n"                                      \
                  "*** 4. Dump tree with dot\n"

#define PARTING  "Have a nice night, stranger!\n"


/**
 * \brief 'Guess' the subject function.
 * \param [in]  node  Pointer to node of tree.
 * \param [in, out]   IsOk     State variable.
 * \return pointer to leaf node.
 */
ad6::Node * ad6::Aki::Guess( Node *node, int *IsOk )
{

  char buf[ANSWER_MAX] = {};

  printf("%s?\n", node->quest);
  scanf("%s", buf);

  if (stricmp("Yes", buf) == 0)
  {
    if (node->right != nullptr)
      return Guess(node->right, IsOk);
    *IsOk = 1;
    return node;
  }
  else if (stricmp("No", buf) == 0)
  {
    if (node->left != nullptr)
      return Guess(node->left, IsOk);
    *IsOk = 0;
    return node;
  }

  printf("So strange answer, I don't understand  what '%s' mean\n", buf);
  printf("Try again :) \n");
  return Guess(node, IsOk);
} /* End of 'Guess' funciton */


/**
 * \brief Craete node function.
 */
bool ad6::Aki::CreateNodes( Node *prnt )
{
  prnt->left = new Node;
  prnt->right = new Node;

  return true;
} /* End of 'CreateNode' function */

/**
 * \brief Alinator play function
 */
void ad6::Aki::Play( void )
{
  int IsWin = 0;
  Node *node = Guess(root, &IsWin);
  if (IsWin)
    printf("It was so easy\n");
  else
  {
    printf("OK, you win.....\nSo, who it was?\n");
    CreateNodes(node);
    strcpy(node->left->quest, node->quest);
    scanf("\n%[^\n]", node->right->quest);
    printf("Hmmm. And what %s has, that %s doesn't has?\n", 
           node->right->quest, node->left->quest);
    scanf("\n%[^\n]", node->quest);
    printf("OK. I will remember that %s %s, and %s don't\n",
           node->right->quest, node->quest, node->left->quest);
  }
} /* End of 'Play' funciton */


/**
 * \brief Akinator process loop function.
 */
void ad6::Aki::ProcessLoop( void )
{
  printf(GREETING);
  printf("\n\n");
  int promt = 0;
  char buf[ANSWER_MAX] = {};
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

      while (1)
      {
        printf("Let me guess what you think about.\n");
        printf("Guess somebody or something......\n");
        Play();
        printf("Try again?\n");
        char p = 0;
        scanf("\n%c", &p);
        if (p == 'n' || p == 'N')
          break;
      }


      break;
    case 2:
      printf("Input file name to save tree:\n");
      scanf("\n%s", buf);
      if (!SaveTree(buf))
        printf("ERROR\n");
      break;
    case 3:
      printf("Input file name to load tree from:\n");
      scanf("\n%s", buf);
      if (!ReadTree(buf))
        printf("ERROR\n");
      break;
    case 4:
      printf("Input file name to dump tree:\n");
      scanf("\n%s", buf);
      if (!Dump(buf))
        printf("ERROR!!!\n");
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
bool ad6::Aki::SaveTree( const char filename[] )
{
  assert(filename != nullptr);

  FILE *tree = fopen(filename, "w");

  if (tree == nullptr)
  {
    printf("Oh, some errors with openning file \"%s\"", filename);
    return false;
  }

  PrintTree(tree, root);

  fclose(tree);
  return true;
} /* End of 'SaveTree' function */


/**
 * \brief Print tree to opened file function.
 * \param [in] node pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::Aki::PrintTree( FILE *f, Node *node )
{
  assert(f != nullptr);
#define IsLeaf(node)  ((node)->right == nullptr && (node)->left == nullptr)

  fprintf(f, "{\"%s%c\"", node->quest, IsLeaf(node) ? '!' : '?');
  if (node->right != nullptr)
    PrintTree(f, node->right);
  if (node->left != nullptr)
    PrintTree(f, node->left);

  fprintf(f, "}");


#undef IsLeaf
  return true;
} /* End of 'PrintTree' function */

/**
 * \brief Read tree from file function.
 * \param [in] pointer to file structure
 * \param [in]  filename  Name of a file to read from.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::Aki::ReadTree( const char filename[] )
{
  assert(filename != nullptr);

  FILE *tree = fopen(filename, "r");

  if (tree == nullptr)
  {
    printf("Oh, some errors with openning file \"%s\"", filename);
    return false;
  }

  BuildTree(tree, root);

  fclose(tree);
  return true;
} /* End of 'ReadTree' function */

/**
 * \brief Build tree from opened file function.
 * \param [in] pointer to file structure
 * \param [in] node pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::Aki::BuildTree( FILE *f, Node *node )
{
  assert(f != nullptr);
  assert(node != nullptr);

  char IsLeaf = 0;
  fscanf(f, "{\"%[^!?]%c\"", node->quest, &IsLeaf);
  if (IsLeaf == '?')
  {
    CreateNodes(node);
    BuildTree(f, node->right);
    BuildTree(f, node->left);
  }
  fscanf(f, "%*c");

  return true;
} /* End of 'BuildTree' function */

/**
 * \brief Alinator dump function.
 * \param filename Name of a file to dump.
 */
bool  ad6::Aki::Dump( const char filename[] )
{
  assert(filename != nullptr);

  FILE *dmp = fopen(filename, "w");

  if (dmp == nullptr)
  {
    printf("Oh, some errors with openning file \"%s\"", filename);
    return false;
  }

  fprintf(dmp, "digraph Dump \n{\n");

  RecDump(dmp, root);

  fprintf(dmp, "}");

  fclose(dmp);
  
  char buf[ANSWER_MAX] = {};
  sprintf(buf, "dot -Tpng %s -oDump.png", filename);

  system(buf);
  return true;
} /* End of 'Dump' funciton */


void ad6::Aki::RecDump( FILE *dmp, Node *node )
{ 
  assert(dmp != nullptr);

#define IsLeaf(node)  ((node)->right == nullptr && (node)->left == nullptr)

  if (IsLeaf(node))
    return;
  fprintf(dmp, "\"%s?\" -> \"%s%c\" [label = \"yes\"];\n", node->quest, node->right->quest, IsLeaf(node->right) ? '.' : '?');
  fprintf(dmp, "\"%s?\" -> \"%s%c\" [label = \"no\"];\n", node->quest, node->left->quest, IsLeaf(node->left) ? '.' : '?');

  RecDump(dmp, node->right);
  RecDump(dmp, node->left);

#undef IsLeaf

} /* End of 'RecDump' function */
