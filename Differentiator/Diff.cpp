#include "Diff.h"

#define TEX_DMP \
"\\documentclass[6pt,a4paper,fleqn]{article} \n"                    \
"\\usepackage[utf8]{inputenc}                                     \n"\
"\\usepackage{amssymb, amsmath, multicol}                         \n"\
"\\usepackage[russian]{babel}                                     \n"\
"                                                                 \n"\
"\\oddsidemargin=-15.4mm                                          \n"\
"\\textwidth=190mm                                                \n"\
"\\headheight=-32.4mm                                             \n"\
"\\textheight=277mm                                               \n"\
"\\tolerance=100                                                  \n"\
"\\parindent=0pt                                                  \n"\
"\\parskip=8pt                                                    \n"\
"\\pagestyle{empty}                                               \n"\
"\\begin{document}                                                \n"

#define TEX_END "\n\\end{document}"

#define GREETING  "Hello. I'm an artificial intelligence.\n"                              \
                  "My dad is Andrey Derzhavin, and he has his own group in vk\n"          \
                  "Subscribe, if you want to know more about my dad's phystech life\n"    \
                  "https://vk.com/andryha_mipt \n\n"                                      \
                  "*** Here what can I do: \n"                                            \
                  "*** 0. Exit (bye bye)\n"                                               \
                  "*** 1. Diff \n"                                                        \
                  "*** 2. Dump tree with dot\n"                                           \
                  "*** 3. Dump tree with tex\n"                                           \
                  "*** 4. Fun dump tree with tex\n"                          


#define PARTING  "Have a nice night, stranger!\n"


#define TEX_GREETING "Hello, let's get the derrivative of this simpliest function:\\\\"

#define TEX_PARTING "It was so easy, isn't it?))"

const char *ad6::phrases[] =
  {
    "It's obviously, that",
    "As we all know from Petrovich, pg. 115",
    "After simple transformations",
    "Let's notice, that",
    "As in previous example",
    "As everybody knows",
    "After some magic, we get",
    "Let's pet Poltorashka and calculate that",
    "Neglecting of infinity small values, we get",
    "Isn't so hard to notice, that",
    "My sister calculated, that. (She is in 1st form)",
    "If you can't understand this, you are dumb",
    "As we all know from mathanalisys course",
    "From the Farid Haidari's theorem follows, that",
    "As every self-sufficient human knows since first grade:"
  };

// Default constructor
ad6::tree::tree( void ) : root(),
                   diff(nullptr),
                   buf(nullptr),
                   buf_size(0),
                   symp_counter(0),
                   is_diff_init(false),
                   toks(nullptr),
                   toks_size(0)
{
}

// Default destructor
ad6::tree::~tree( void )
{
  if (diff != nullptr)
  {
    for (size_t i = 0; i < par.var_size(); i++)
    {
      if (diff[i] != nullptr)
        delete diff[i];
    }
    delete[] diff;
  }
  if (root != nullptr)
  {
    delete root;
    root = nullptr;
  }
  if (buf != nullptr)
  {
    delete[] buf;
    buf = nullptr;
  }
  if (toks != nullptr)
  {
    delete[] toks;
    toks = nullptr;
  }
}

/**
 * \brief Pars text to tokens function.
 */
void ad6::tree::_pre_par( void )
{
  toks = new token[buf_size + 1];
  unsigned str_counter = 0, pos_counter = 0;
  int pos = 0;

  for (unsigned i = 0; i < buf_size; i++)
  {
    if (isspace(buf[i]))
    {
      if (buf[i] == '\n')
      {
        pos_counter = 0;
        str_counter++;
      }
      continue;
    }

    if (isdigit(buf[i]))
    {
      toks[toks_size++] = token(_getNum(&i), str_counter + 1, pos_counter + 1);
      continue;
    }

    if (isalpha(buf[i]))
    {
      unsigned end_word = _getWord(&i);

      toks[toks_size++] = token(TOK_STR, 0, str_counter + 1, pos_counter + 1, buf + i, end_word);
      i += end_word - 1;
      continue;
    }
    toks[toks_size++] = token(TOK_SMB, 0, str_counter + 1, pos_counter + 1, buf + i, 1);
  }
  toks[toks_size++] = token(TOK_NUL, 0, str_counter + 1, buf_size);
} /* End of '_pre_par' function */

/**
 * \brief Get word token function.
 * \param [in] pos     Pointer to position.
 * \return new position.
 */
unsigned ad6::tree::_getWord( unsigned *pos )
{
  unsigned new_pos = 0;
  
  do
  {
    new_pos++;
  } while (isalpha((unsigned char)buf[*pos + new_pos]));

  return new_pos;
} /* End of '_getWord' function */

/**
 * \brief Get number token function.
 * \param [in] pos     Pointer to position.
 * \return number value.
 */
double ad6::tree::_getNum( unsigned *pos )
{
  double num = 0;
  unsigned add_pos = 0;
  
  if (sscanf(buf + *pos, "%lg%n", &num, &add_pos) != 1)
    return NAN;

  *pos = *pos + add_pos - 1;
  return num;
} /* End of '_getNum' function */

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

  buf = FillBuf(filename, &buf_size);

  char IsOk = 0;
  size_t pos = 0;
  TREE_ASSERT(buf != nullptr, "Error with opening file")

  if (root != nullptr)
    delete root;

  _pre_par();

  root = par.getG(toks);

  return true;
} /* End of 'read_tree' function */


size_t ad6::IsCorrBraces( char *buf, size_t size )
{
  int IsCorr = 0;
  size_t i = 0;

  for (i = 0; i < size; i++)
  {
    if (buf[i] == '(')
      IsCorr++;
    else if (buf[i] == ')')
    {
      if (IsCorr <= 0)
        return i + 1;
      IsCorr--;
    }
  }
    if (IsCorr == 0)
      return 0;
    return i; 
}
/**
 * \brief Build  tree from buffer function.
 * \param [in] pointer to buffer
 * \param [in] node pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::tree::build_tree( char *buf )
{
  return true;
} /* End of 'build_tree' function */

/**
 * \brief Find operator number function.
 * \brief string ppointer to string.
 * \return number of the operator if it was find.
 * \return -1 otherwise.
*/
int ad6::tree::find_op( char sym )
{

  #define DEF_OP(num, name, calc, diff)                 \
     if (sym == num)                              \
       return num;

  #include "cmd.h"

  #undef DEF_OP
  return -1;
} /* End of 'find_op' function */


/**
 * \brief tree dump function.
 * \param filename Name of a file to dump.
 */
bool  ad6::tree::dump( const char filename[], node *nd ) const
{
  TREE_ASSERT(filename != nullptr, "Incorrect file name");

  char buffer[ANSWER_MAX] = {};

  sprintf(buffer, "%s.dot", filename);

  FILE *dmp = fopen(buffer, "wb");

  if (dmp == nullptr)
  {
    printf("Oh, some errors with openning file %s\n", buffer);
    return false;
  }

  fprintf(dmp, "digraph %s \n{\n", filename);
  
  rec_dump(dmp, nd);
  
  fprintf(dmp, "}\n");
  

  fclose(dmp);
 
  sprintf(buffer, "dot -Tpng %s.dot -o%s.png", filename, filename);

  system(buffer);
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
      fprintf(dmp, "\"%p\" [label = \"%c\"];\n", node, node->num);
    }
    if (node->right != nullptr)
    {
      fprintf(dmp, "\"%p\" -> ", node);
      rec_dump(dmp, node->right);
      fprintf(dmp, "\"%p\" [label = \"%c\"];\n", node, node->num);
    }
    break;
  case TYPE_NUMBER:
    fprintf(dmp, "\"%p\";\n\"%p\" [label = \"%lg\"];\n", node, node, node->value);
    break;
  case TYPE_VAR:
    fprintf(dmp, "\"%p\";\n\"%p\" [label = \"", node, node);
    node->name.print_in_file(dmp);
    fprintf(dmp, "\"];\n");
    break;
  case TYPE_FUNC:
    fprintf(dmp, "\"%p\" -> ", node);
    rec_dump(dmp, node->right);
    fprintf(dmp, "\"%p\" [label = \"", node);
    node->name.print_in_file(dmp);
    fprintf(dmp, "\"];\n");
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


/**
 * \brief Fill diff array with nullptr values function
*/
void ad6::tree::_fill_diff( void )
{
  for (size_t i = 0; i < par.var_size(); i++)
    diff[i] = nullptr;
} /* End of '_fill_diff' function */


/**
 * \brief Create a differentiating tree's array function
 */
void ad6::tree::_diff_init( void )
{
  if (par.var_size() == 0)
  {
    diff = nullptr;
    return;
  }
  diff = new node*[par.var_size()];
  _fill_diff();
} /* End of '_diff_init' function */

/**
 * \brief Create differntiate tree function
 */
bool ad6::tree::tree_diff( const char var[] )
{
  TREE_ASSERT(root != nullptr, "Root was nullptr");

  size_t var_num = 0;

  if ((var_num = par.find_var(var)) == -1)
    return false;

  tree_diff(var_num);
  
  return true;
} /* End of 'tree_diff' function */

bool ad6::tree::tree_diff( size_t var_num )
{
  TREE_ASSERT(root != nullptr, "Root was nullptr");

  if (!is_diff_init)
  {
    _diff_init();
    is_diff_init = true;
  }

  diff[var_num] = &rec_diff(*root, var_num);
  
  return true;
} /* End of 'tree_diff' function */

/**
 * \brief Simplify differentiated tree function.
 */
void ad6::tree::_diff_simpl( void )
{
  for (size_t i = 0; i < par.var_size(); i++)
    _simplifier(&diff[i]);
} /* End of '_diff_simpl' function */

/**
 * \brief Recursive differentiator of tree. 
 */
ad6::node & ad6::tree::rec_diff( node &nd, size_t var_num )
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

  #include "cmd.h"

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
 
    #include "func.h"

    default:
      TREE_ASSERT(0, "Unrecognized function");
    }

  #undef DEF_FNC
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
double ad6::tree::rec_calc( node *nd ) const
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
      #include "cmd.h"
    default:
      printf("Incorrect operator num : %d\n", nd->num);
    }
    break;
  case TYPE_FUNC:
    switch (nd->num)
    {
     
    #include "func.h"

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
bool ad6::tree::_simplifier( node **nd )
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
bool ad6::tree::_calc_tree( node *nd )
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
ad6::node* ad6::tree::_rec_symp( node *nd )
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
    nd->right = _rec_symp(nd->right);
    return nd;
  case TYPE_OPERATOR:
    
#define DEF_SMP(num, simp)                               \
     case num:                                           \
      simp;                                              \
      break;

    switch (nd->num)
    {
    
    #include "simp.h"

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
bool ad6::tree::_is_calc( node *nd ) const
{
  if (nd->type == TYPE_VAR)
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
 * \brief Dump in .tex file function (funny version)
 * \param [in]  filename              Name of a file to dump.
 */
void ad6::tree::tex_fun_dump( const char filename[] )
{
  TREE_ASSERT(filename != nullptr, "Incorrect filename");

  size_t phr_size = sizeof(phrases) / sizeof(phrases[0]);
  char buf[ANSWER_MAX] = {};
  sprintf(buf, "%s.tex", filename);

  srand(clock());

  if (rand() % 2)
  {
    printf("Today is a bad day for differentiate, I won't do this\n");
    return;
  }

  FILE *tex = fopen(buf, "wb");
  
  TREE_ASSERT(tex != nullptr, "Cannot open file for tex ");

  fprintf(tex, TEX_DMP);

  fprintf(tex, TEX_GREETING);


  fprintf(tex, "$$\n f \\left(");
    for (size_t i = 0; i < par.var_size(); i++)
    {
      par.get_var(i).print_in_file(tex);
      if (i != par.var_size() - 1)
        fprintf(tex, ", ");
    }
  fprintf(tex, " \\right) = ");

  _tex_rec(tex, root);
  fprintf(tex, " $$\n\\\\");

#define PRINT_DER(var)                                       \
  fprintf(tex, "$$\nf'_{");                                      \
  par.get_var(var).print_in_file(tex);                       \
  fprintf(tex, "} \\left( ");                                \
  for (size_t i = 0; i < par.var_size(); i++)                \
    {                                                        \
      par.get_var(i).print_in_file(tex);                     \
      if (i != par.var_size() - 1)                           \
        fprintf(tex, ", ");                                  \
    }                                                        \
  fprintf(tex, " \\right) = ");

#define PRINT  {                                                             \
                 srand(clock());                                             \
                 fprintf(tex, "\\\\\n%s\\\\\n", phrases[rand() % phr_size]); \
                 PRINT_DER(i);                                               \
                 _tex_rec(tex, diff[i]);                                     \
                 fprintf(tex, "\n$$\\\\");                                   \
               }

  for (size_t i = 0; i < par.var_size(); i++)
  {
    symp_counter = 0;
    int prev_cnt = 1;
    while (prev_cnt != symp_counter)
    {
      prev_cnt = symp_counter;
      diff[i] = _rec_symp(diff[i]);
      if (prev_cnt != symp_counter)
        PRINT
    }
  }
  fprintf(tex, TEX_PARTING);
  fprintf(tex, TEX_END);

#undef PRINT

#undef PRINT_DER
  fclose(tex);
} /* End of 'tex_fun_dump' function */

/**
 * \brief Dump tree in .tex file function.
 * \param [in] filename Name of a file to save.
 * \brief [root]   pointer to tree's root.
*/
void ad6::tree::tex_dump( const char filename[], node* root )
{
  TREE_ASSERT(filename != nullptr, "Incorrect filename");
  TREE_ASSERT(root != nullptr, "Incorrect tree root");

  char buffer[ANSWER_MAX] = {};
  sprintf(buffer, "%s.tex", filename);

  FILE *tex = fopen(buffer, "wb");
  
  TREE_ASSERT(tex != nullptr, "Cannot open file for tex ");

  fprintf(tex, TEX_DMP);

  fprintf(tex, "$\n ");

  _tex_rec(tex, root);
  fprintf(tex, " $\n");
  fprintf(tex, TEX_END);

  fclose(tex);


  sprintf(buffer, "pdflatex %s.tex", filename);

//  system(buffer);

  sprintf(buffer, "%s.pdf", filename);
  //system(buffer);
} /* End of 'tex_dump' function */

/**
 * \brief Recursive dump tree in .tex file function.
 * \param [in] f Pointer ro opened file's structure.
 * \brief [nd]   pointer to tree's node.
*/
void ad6::tree::_tex_rec( FILE *f, node *nd )
{
  TREE_ASSERT(f != nullptr, "File wasn't open");
  TREE_ASSERT(nd != nullptr, "Node was nullptr");
    

  switch (nd->type)
  {
  case TYPE_OPERATOR:
    _op_tex_rec(f, nd);
    break;
  case TYPE_NUMBER:
    fprintf(f, "%lg", nd->value);
    break;
  case TYPE_VAR:
    nd->name.print_in_file(f);
    break;
  case TYPE_FUNC:
    if (nd->num == 11)
      fprintf(f, "e^{");
    else
    {
      fprintf(f, "\\");
      nd->name.print_in_file(f);
      if (nd->num == 6)
        fprintf(f, "{");
      else
        fprintf(f, " \\left( ");
    }
    _tex_rec(f, nd->right); 
    if (nd->num == 6)
      fprintf(f, "}");
    else if (nd->num != 11)
      fprintf(f, " \\right) ");
    else
      fprintf(f, "} ");
    break;
  default:
    printf("Unrecognised type : %d", nd->type);
    break;
  }

} /* End of '_tex_rec' function */


/**
 * \brief Print operators in .tex file function.
 * \param [in]        f          pointer to file structure
 * \param [in, out]   nd         tree's node.
 */
void ad6::tree::_op_tex_rec( FILE *f, node *nd )
{
  TREE_ASSERT(f != nullptr, "File wasn't open");
  TREE_ASSERT(nd != nullptr, "Node was nullptr");

#define CHECK(nde, str)                                                 \
  if (nd->##nde->type == TYPE_NUMBER && nd->##nde->value < 0)        \
      fprintf(f, str);

  #define CHECK_LEAF(nde, lef, rig)                                                                            \
  if (nd->##nde->type == TYPE_OPERATOR && (nd->##nde->num == '+' || nd->##nde->num == '-'))        \
    {                                                                                              \
      fprintf(f, " %s ", lef);                                                                            \
      _tex_rec(f, nd->##nde);                                                                    \
      fprintf(f, " %s ", rig);                                                                            \
    }                                                                                              \
    else                                                                                           \
      _tex_rec(f, nd->##nde);

  if (nd->num == '^')
  {
    if (nd->left->type == TYPE_OPERATOR && nd->left->num != '^')
    {
      fprintf(f, " \\left( ");
      _tex_rec(f, nd->left);
      fprintf(f, " \\right) ");
    }
    else
      _tex_rec(f, nd->left);
    fprintf(f, " ^{");
    _tex_rec(f, nd->right);
    fprintf(f, "}");
    return;
  }
  if (nd->num == '/')
  {
    fprintf(f, " \\dfrac{ ");
    _tex_rec(f, nd->left);
    fprintf(f, " }{ ");
    _tex_rec(f, nd->right);
    fprintf(f, " }");
    return;
  }
  if (nd->num == '*')
  {
    CHECK(left, " \\left( ");
    CHECK_LEAF(left, " \\left( ", " \\right) ");
    CHECK(left, " \\right) ");

    fprintf(f, " \\cdot ");

    CHECK(right, " \\left( ");
    CHECK_LEAF(right, "\\left( ",  " \\right)");
    CHECK(right, " \\right) ");
    return;
  }
  if (nd->left->type == TYPE_NUMBER && nd->left->value == 0 && nd->num == '-' 
      && (nd->right->type != TYPE_NUMBER || nd->right->value != 0))
  {
    fprintf(f, " \\left( -");
    _tex_rec(f, nd->right);
    fprintf(f, " \\right) ");
    return;
  }


  _tex_rec(f, nd->left);
  fprintf(f, " %c ", nd->num);
  _tex_rec(f, nd->right);
} /* End of '_op_tex_rex' function */

void ad6::tree::process_loop( void )
{
  printf(GREETING);
  char *buff = nullptr;
  char *ans = nullptr;
  bool IsRead = false;
  while (1)
  {
    int num = 0;
    printf("Input number to start: ");
    scanf("%d", &num);

    switch (num)
    {
    case 0:
      printf(PARTING);
      return;
    case 1:
      if (IsRead)
        break;
      IsRead = true;
      buff = InputAnswer("Input filename to read function from: ");
      if (read_tree(buff))
        printf("build success\n");
      while (1)
      {
        buff = InputAnswer("Input a variable to differentiate or 0 to quit: ");
        if (buff[0] == '0')
          break;
        if (!tree_diff(buff))
          printf("Incorrect variable, try again\n");
      }
      break;
      case 2:
        buff = strdup(InputAnswer("Input filename to dump tree with .dot: "));
        printf("1 - root, 2 - differetiated tree ");
        scanf("%d", &num);
        if (num == 1)
          dump(buff, root);
        else if (num == 2)
        {
          if (diff != nullptr)
          {
            ans = InputAnswer("Input variable name: ");
            size_t num = par.find_var(ans);
            if (num == -1)
              printf("\nNo variables such this\n");
            else
            {
              _simplifier(&diff[num]);
              dump(buff, diff[num]);
            }
          }
        }
        free(buff);
        break;
      case 3:
        buff = strdup(InputAnswer("Input filename to dump tree with tex: "));
        printf("1 - root, 2 - differetiated tree ");
        scanf("%d", &num);
        if (num == 1)
          tex_dump(buff, root);
        else if (num == 2)
        {
          if (diff != nullptr)
          {
            ans = InputAnswer("Input variable name: ");
            size_t num = par.find_var(ans);
            if (num == -1)
              printf("No variables such this\n");
            else
            {
              _simplifier(&diff[num]);
              tex_dump(buff, diff[num]);
            }
          }
        }
        free(buff);
        break;

      case 4:
        buff = InputAnswer("Input filename to dump tree with tex: ");
        printf("\n processing ....\n");
        tex_fun_dump(buff);
        printf("Dump succes, enjoy it\n");
        break;
      default:
        printf("Unrecognized switch %d\n Please, try again.\n", num);
        break;
    }
  }
}

/**
 * \brief Find variable in tree function.
 * \param [in] start Pointer to tree's node.
 * \param [in] var_num Number of variable in variables array.
*/
bool ad6::tree::_find_var_tree( node *start, size_t var_num ) const
{
  if (start->type == TYPE_VAR && start->num == var_num)
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

/**
 * \brief Kill laba function
 */
double ad6::tree::laba_kill( const char filename[] )
{
  TREE_ASSERT(filename != nullptr, "Incorrect filename");

  read_tree(filename);

  double answ = 0;
  

  _vars_init();
  _get_acc();

  for (size_t i = 0; i < par.var_size(); i++)
  {
    tree_diff(i);
    _simplifier(&diff[i]);
    _tree_vars_init(diff[i]);
    double res = rec_calc(diff[i]);
    answ += res * res * accuracies[i] * accuracies[i];
  }

  return sqrt(answ);
}

void ad6::tree::_tree_vars_init( node *start )
{
  TREE_ASSERT(start != nullptr, "Node was nullptr");

  if (start->left != nullptr)
    _tree_vars_init(start->left);

  if (start->right != nullptr)
    _tree_vars_init(start->right);

  if (start->type != TYPE_VAR)
    return;

  start->type = TYPE_NUMBER;
  start->value = par[start->num].value;
}

void ad6::tree::_vars_init( void )
{
  for (size_t i = 0; i < par.var_size(); i++)
  {
    double val = 0;
    std::cout << "Input '" << par.get_var(i) << "' value:\n";
    scanf("%lg", &val);
    par[i].value = val;
  }
}

void ad6::tree::_get_acc( void )
{
  accuracies.resize(par.var_size());

  for (size_t i = 0; i < par.var_size(); i++)
  {
    double ac = 0;
    std::cout << "Input accuracy for '" << par.get_var(i) << "' variable:\n";
    while (1)
    {
      scanf("%lg", &ac);
      if (ac < 0)
        printf("Accuracy always > 0, try again))\n");
      else
        break;
    }
    accuracies[i] = ac;
  }
}

void ad6::tree::write_answ( const char filename[], double answ )
{
  TREE_ASSERT(filename != nullptr, "Incorrect filename");

  FILE *f = fopen(filename, "wb");

  TREE_ASSERT(f != nullptr, "Error with opening file");
  
  fprintf(f, "Accuracy = %lg\n", answ);
  fclose(f);
}
#undef TEX_DMP

#undef TEX_END
