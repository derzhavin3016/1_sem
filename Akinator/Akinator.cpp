#include "Akinator.h"

#define AKI_LOCATION  __LINE__, __FILE__, __FUNCSIG__

#define AKI_ASSERT(cond, err)           if (!(cond))                               \
                                        {                                          \
                                          throw Error{err, AKI_LOCATION};          \
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
 * \brief 'Guess' the subject function.
 * \param [in]  node  Pointer to node of tree.
 * \param [in, out]   IsOk     State variable.
 * \return pointer to leaf node.
 */
ad6::Node * ad6::Aki::Guess( Node *node, int *IsOk )
{
  char *buf = InputAnswer("%s?\n", node->quest);
  
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
 * \brief Akinator play function
 * \warning Maximum string length is 1024.
 */
void ad6::Aki::Play( void )
{
  int IsWin = 0;
  Node *node = Guess(root, &IsWin);
  if (IsWin)
    printf("It was so easy\n");
  else
  {
    char *buf = strdup(InputAnswer("OK, you win.....\nSo, who it was?\n"));

    AKI_ASSERT(buf != nullptr, "strdup returned nullptr\n");

    int num = Base.FindValue(buf);
    if (num != -1)
    {
    
      printf("OOOOOOPS, I already know %s\n", buf);
      free(buf);
    }
    else
    {
      CreateNodes(node);
      node->left->quest = node->quest;
      node->right->quest = buf;
      Base.Push_tail(buf);
      node->right->parent = node->left->parent = node;

      node->quest = strdup(InputAnswer("Hmmm. And what %s has, that %s doesn't has?\n", 
             node->right->quest, node->left->quest));
      printf("OK. I will remember that %s is %s, and %s don't\n",
             node->right->quest, node->quest, node->left->quest);
    }
    
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
  char *buf = nullptr;

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
      SaveTree(InputAnswer("Input file name to save tree:\n"));
      break;
    case 3:
      ReadTree(InputAnswer("Input file name to load tree from:\n"));
      break;
    case 4:
      Dump(InputAnswer("Input file name to dump tree (only name):\n"));
      printf("Dump success\n");
      break;
    case 5:
      Character(InputAnswer("Input name of the object:\n"));
      break;
    case 6:
      buf = strdup(InputAnswer("Input name of the first object:\n"));
      InputAnswer("Input name of the second object:\n");
      
      AKI_ASSERT(buf != nullptr, "strdup returned nullptr");

      Compare(buf, InputAnswer(nullptr));
      free(buf);
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
bool ad6::Aki::SaveTree( const char filename[] ) const
{
  AKI_ASSERT(filename != nullptr, "Incorrect file name");

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
bool ad6::Aki::PrintTree( FILE *f, Node *node ) const
{
  AKI_ASSERT(f != nullptr, "File not opened\n");
  AKI_ASSERT(node != nullptr, "node was nullptr");

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
  AKI_ASSERT(filename != nullptr, "Incorrect file name");

  FILE *tree = fopen(filename, "rb");

  if (tree == nullptr)
  {
    printf("Oh, some errors with openning file \"%s\"", filename);
    return false;
  }
  Base.Kill();
  if (root != nullptr)
    delete root;

  root = new Node;

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
  AKI_ASSERT(f != nullptr, "File not opened");
  AKI_ASSERT(node != nullptr, "node was nullptr");

  char IsLeaf = 0;
  node->quest = new char[ANSWER_MAX];
  fscanf(f, "{\"%[^!?]%c\"", node->quest, &IsLeaf);
  if (IsLeaf == '?')
  {
    CreateNodes(node);
    node->right->parent = node->left->parent = node;
    BuildTree(f, node->right);
    BuildTree(f, node->left);
  }
  else
  {
    Base.Push_tail(node->quest);
  }
  fscanf(f, "%*c");

  return true;
} /* End of 'BuildTree' function */

/**
 * \brief Akinator dump function.
 * \param filename Name of a file to dump.
 */
bool  ad6::Aki::Dump( const char filename[] ) const
{
  AKI_ASSERT(filename != nullptr, "Incorrect file name");

  char buf[ANSWER_MAX] = {};

  sprintf(buf, "%s.dot", filename);

  FILE *dmp = fopen(buf, "w");

  if (dmp == nullptr)
  {
    printf("Oh, some errors with openning file %s\n", buf);
    return false;
  }

  fprintf(dmp, "digraph %s \n{\n", filename);

  if (root->left == nullptr)
    fprintf(dmp, "\"%s\"", root->quest);
  else
    RecDump(dmp, root);

  fprintf(dmp, "}");

  fclose(dmp);
 
  sprintf(buf, "dot -Tpng %s.dot -o%s.png", filename, filename);

  system(buf);
  return true;
} /* End of 'Dump' funciton */

/**
 * \brief Recursion dump tree function.
 * \param dmp  Pointer to opened file structure.
 * \param node  Pointer to node.
 */
void ad6::Aki::RecDump( FILE *dmp, Node *node ) const
{ 
  AKI_ASSERT(dmp != nullptr, "File not opened");
  AKI_ASSERT(node != nullptr, "node was nullptr");

#define IsLeaf(node)  ((node)->right == nullptr && (node)->left == nullptr)

  if (IsLeaf(node))
    return;
  fprintf(dmp, "\"%s?\" -> \"%s%c\" [label = \"yes\"];\n", node->quest, node->right->quest, IsLeaf(node->right) ? '.' : '?');
  fprintf(dmp, "\"%s?\" -> \"%s%c\" [label = \"no\"];\n", node->quest, node->left->quest, IsLeaf(node->left) ? '.' : '?');

  RecDump(dmp, node->right);
  RecDump(dmp, node->left);

#undef IsLeaf

} /* End of 'RecDump' function */


/**
 * \brief Print characteristic of the object.
 * \param name  Name of the object.
 */
void ad6::Aki::Character( const char name[] ) const
{
  AKI_ASSERT(name != nullptr, "Incorrect character name");

  Node *node = Find(name, root);
  if (node == nullptr)
  {
    printf("I don't know who it is...\n");
    return;
  }
  printf("The %s is ", node->quest);

  Stack<char *> chr = {};
  StackInit(&chr);
  Stack<int> route = {};
  StackInit(&route);
  FillRoute(&route, &chr, node);

  int size = chr.size;
  char *buf = nullptr;
  int answer = 0;
  for (int i = 0; i < size; i++)
  {
    StackPop(&route, &answer);
    StackPop(&chr, &buf);
    printf("%s%s%s", answer ? " " : " not ", buf, i == size - 1 ? ".\n" : " and");
  }

  StackClose(&chr);
  StackClose(&route);
} /* End of 'Character' function */

/**
 * \brief Compare two objects function.
 */
bool ad6::Aki::Compare( const char name1[], const char name2[] ) const
{
  AKI_ASSERT(name1 != nullptr, "Incorrect first character name");
  AKI_ASSERT(name2 != nullptr, "Incorrect second character name");

  if (strcmp(name1, name2) == 0)
  {
    printf("Seems they are the same..\n");
    return true;
  }

#define FIND_NAME(node, name)                                 \
  Node *node = Find(name, root);                              \
  if (node == nullptr)                                        \
  {                                                           \
    printf("I don't know who it is - '%s'\n", name);          \
    return false;                                             \
  }

#define FILL_ROUTE(quest, answ, node)                          \
  Stack<char *> quest;                                        \
  StackInit(&quest);                                          \
  Stack<int> answ;                                            \
  StackInit(&answ);                                           \
  FillRoute(&answ, &quest, node);

  FIND_NAME(node1, name1);

  FIND_NAME(node2, name2);

  FILL_ROUTE(quest1, answ1, node1);
  FILL_ROUTE(quest2, answ2, node2);

#define POP_ROUTE(quest, answ, route, buf)    \
  StackPop(&answ, &route);                    \
  StackPop(&quest, &buf);


  char *buf1 = nullptr, *buf2 = nullptr;
  int route1 = 0, route2 = 0;

  bool IsDiff = false;

  POP_ROUTE(quest1, answ1, route1, buf1);
  POP_ROUTE(quest2, answ2, route2, buf2);
  if (route1 == route2)
    printf("The %s and %s are both", node1->quest, node2->quest);
  while (route1 == route2)
  {
    IsDiff = true;
    printf("%s%s and", route1 ? " " : " not ", buf1);
    POP_ROUTE(quest1, answ1, route1, buf1);
    POP_ROUTE(quest2, answ2, route2, buf2);
  }
  if (IsDiff)
    printf(" they have differnce:\n");
  printf("The %s %s, but %s don't\n", route1 ? name1 : name2, route1 ? buf1 : buf2, route1 ? name2 : name1);

  StackClose(&quest1);
  StackClose(&quest2);
  StackClose(&answ1);
  StackClose(&answ2);
  return true;
} /* End of 'Compare' function */


/** 
 * \brief Fill routes stack function.
 * \param answer  pointer to stack with answers.
 * \param quest   pointer to stack with questions.
 * \param node   pointer to Node.
 */
bool ad6::Aki::FillRoute( Stack<int> *answer, Stack<char *> *quest, Node *node ) const
{
  AKI_ASSERT(answer != nullptr, "Pointer to answers stack was nullptr\n");
  AKI_ASSERT(quest != nullptr, "Pointer to questions stack was nullptr\n");
  AKI_ASSERT(node != nullptr, "Pointer to node was nullptr\n");

  Node **p_node = &(node->parent);
  Node **prev_p = &node;
  while (*p_node != nullptr)
  {
    if (*prev_p == (*p_node)->right)
      StackPush(answer, 1);
    else
      StackPush(answer, 0);
    StackPush(quest, (*p_node)->quest);
    prev_p = p_node;
    p_node = &(*p_node)->parent;
  }
  return true;
} /* End of 'FillRoute' function */

/**
 * \brief Find name in tree function
 * \param name  Name to find.
 * \pararm node pointer to tree node.
 * \return pointer to node if name was find.
 * \return nullptr otherwise.
 */
ad6::Node * ad6::Aki::Find( const char name[], Node *node ) const
{
  AKI_ASSERT(name != nullptr, "Name was nullptr\n");
  AKI_ASSERT(node != nullptr, "Node was nullptr\n");

  if (strcmp(name, node->quest) == 0)
    return node;

  Node *buf = nullptr;
  if (node->right != nullptr)
  {
    buf = Find(name, node->right);
    if (buf != nullptr)
      return buf;
  }


  if (node->left != nullptr)
  {
    buf = Find(name, node->left);
    if (buf != nullptr)
      return buf;
  }

  return nullptr;
} /* End of 'Find' funtction */
