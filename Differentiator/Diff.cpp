#include "Diff.h"

#define TEX_DMP \
"\\documentclass[12pt,a4paper,fleqn]{article}\n"\
"\\usepackage[utf8]{inputenc}                                     \n"\
"\\usepackage{amssymb, amsmath, multicol}                         \n"\
"\\usepackage[russian]{babel}                                     \n"\
"\\usepackage{graphicx}                                           \n"\
"\\usepackage[shortcuts,cyremdash]{extdash}                       \n"\
"\\usepackage{wrapfig}                                            \n"\
"\\usepackage{floatflt}                                           \n"\
"\\usepackage{lipsum}                                             \n"\
"\\usepackage{hyperref}                                           \n"\
"\\usepackage{verbatim}                                           \n"\
"\\usepackage{concmath}                                         \n"\
"\\usepackage{euler}                                            \n"\
"                                                                 \n"\
"\\oddsidemargin=-15.4mm                                          \n"\
"\\textwidth=190mm                                                \n"\
"\\headheight=-32.4mm                                             \n"\
"\\textheight=277mm                                               \n"\
"\\tolerance=100                                                  \n"\
"\\parindent=0pt                                                  \n"\
"\\parskip=8pt                                                    \n"\
"\\pagestyle{empty}                                               \n"\
"\\renewcommand{\\tg}{\\mathop{\\mathrm{tg}}\\nolimits}           \n"\
"\\renewcommand{\\ctg}{\\mathop{\\mathrm{ctg}}\\nolimits}         \n"\
"\\renewcommand{\\arctan}{\\mathop{\\mathrm{arctg}}\\nolimits}    \n"\
"\\newcommand{\\divisible}{\\mathop{\\raisebox{-2pt}{\\vdots}}}   \n"\
"\\RequirePackage{caption2}                                       \n"\
"\\renewcommand\\captionlabeldelim{}                              \n"\
"\\newcommand*{\\hm}[1]{#1\\nobreak\\discretionary{}%             \n"\
"{\\hbox{$\\mathsurround=0pt #1$}}{}}                             \n"\
"                                                                 \n"\
"\\begin{document}                                                \n"

#define TEX_END "\\end{document}"

#define GREETING  "Hello. I'm an artificial intelligence.\n"                              \
                  "My dad is Andrey Derzhavin, and he has his own group in vk\n"          \
                  "Subscribe, if you want to know more about my dad's phystech life\n"    \
                  "https://vk.com/andryha_mipt \n\n"                                      \
                  "*** Here what can I do: \n"                                            \
                  "*** 0. Exit (bye bye)\n"                                               \
                  "*** 1. Diff \n"                                                        \
                  "*** 2. Dump tree with dot\n"                                           \
                  "*** 3. Dump tree with tex\n"                          

#define PARTING  "Have a nice night, stranger!\n"

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

  root = par.getG(buf);

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

  #define DEF_OP(num, name, calc, diff, simp)                 \
     if (sym == num)                              \
       return num;

  #include "cmd.h"

  #undef DEF_OP
  return -1;
} /* End of 'find_op' function */


/**
 * \brief treenator dump function.
 * \param filename Name of a file to dump.
 */
bool  ad6::tree::dump( const char filename[], node *nd ) const
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
 * \brief Create differntiate tree function
 */
void ad6::tree::tree_diff( const char var[] )
{
  TREE_ASSERT(root != nullptr, "Root was nullptr");

  size_t var_num = 0;
  if (par.var_size() == 0)
  {
    diff = new node*(0);
    return;
  }
  diff = new node*[par.var_size()];
  TREE_ASSERT((var_num = par.find_var(var)) != -1, "Incorrect var");

  diff[var_num] = &rec_diff(*root, var_num);
} /* End of 'tree_diff' function */

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

  #define DEF_OP(num, name, calc, diff, simp)              \
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
  
#define DEF_OP(num, name, calc, diff, simp)  \
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
 * \brief Recursive simplifier function
 * \param [in] nd pointer to tree's node.
 * \return true if all is OK.
 * \return false otherwise.
*/
ad6::node* ad6::tree::_rec_symp( node *nd )
{
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
    return nd;
  }

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
    
#define DEF_OP(num, name, calc, diff, simp)              \
     case num:                                           \
      simp;                                              \
      break;

    switch (nd->num)
    {
    
    #include"cmd.h"

    default:
      TREE_ASSERT(0, "Unrecognized operator");
    }


    #undef DEF_OP
    
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

  bool is_l_calc = true;
  if (nd->left != nullptr)
    is_l_calc = _is_calc(nd->left);

  bool is_r_calc = true;
  if (nd->right != nullptr)
     is_r_calc = _is_calc(nd->right);

  return is_r_calc && is_l_calc;
} /* End of '_is_calc' function */

void ad6::tree::tex_dump( const char filename[], node* root )
{
  TREE_ASSERT(filename != nullptr, "Incorrect filename");
  TREE_ASSERT(root != nullptr, "Incorrect tree root");

  char buf[ANSWER_MAX] = {};
  sprintf(buf, "%s.tex", filename);

  FILE *tex = fopen(buf, "wb");
  
  TREE_ASSERT(tex != nullptr, "Cannot open file for tex ");

  fprintf(tex, TEX_DMP);

  fprintf(tex, "$\n \\left(");

  _tex_rec(tex, root);
  fprintf(tex, " \\right) $");
  fprintf(tex, TEX_END);

  fclose(tex);


  sprintf(buf, "pdflatex %s.tex", filename);

  system(buf);

  sprintf(buf, "%s.pdf", filename);
  system(buf);
}

void ad6::tree::_tex_rec( FILE *f, node *nd )
{
  TREE_ASSERT(f != nullptr, "File wasn't open");
  TREE_ASSERT(nd != nullptr, "Node was nullptr");
    

  switch (nd->type)
  {
  case TYPE_OPERATOR:
    if (nd->num == '/')
    
    {
      fprintf(f, "\\dfrac { ");
      _tex_rec(f, nd->left);
      fprintf(f, "}{");
      _tex_rec(f, nd->right);
      fprintf(f, "} ");
      break;
    }
      
    if (nd != root && nd->num != '^')
      fprintf(f, " \\left( ");

    _tex_rec(f, nd->left);
    
    if (nd->num == '*')
      fprintf(f, " \\cdot ");
    else
      fprintf(f, " %c ", nd->num);

    _tex_rec(f, nd->right);
   
    if (nd != root && nd->num != '^')
      fprintf(f, " \\right) ");
    break;
  case TYPE_NUMBER:
    fprintf(f, "%lg", nd->value);
    break;
  case TYPE_VAR:
    nd->name.print_in_file(f);
    break;
  case TYPE_FUNC:
    nd->name.print_in_file(f);
    fprintf(f, " \\left( ");
    _tex_rec(f, nd->right);
    fprintf(f, " \\right) ");
    break;
  default:
    printf("Unrecognised type : %d", nd->type);
    break;
  }

}

void ad6::tree::process_loop( void )
{
  printf(GREETING);
  char *buf = nullptr;
  char *ans = nullptr;
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
      buf = InputAnswer("Input filename to read function from: ");
      if (read_tree(buf))
        printf("build success");
      while (1)
      {
        buf = InputAnswer("Input a variable to differentiate or 0 to quit: ");
        if (buf[0] == '0')
          break;
        tree_diff(buf);
      }
      break;
      case 2:
        buf = InputAnswer("Input filename to dump tree with .dot: ");
        printf("1 - root, 2 - differetiated tree");
        scanf("%d", &num);
        if (num == 1)
          dump(buf, root);
        else if (num == 2)
        {
          if (diff != nullptr)
          {
            ans = InputAnswer("Input variable name: ");
            size_t num = par.find_var(ans);
            if (num == -1)
              printf("No variables such this\n");
            else
              dump(buf, diff[num]);
          }
        }

        break;
      case 3:
        buf = InputAnswer("Input filename to dump tree with tex: ");
        printf("1 - root, 2 - differetiated tree");
        scanf("%d", &num);
        if (num == 1)
          tex_dump(buf, root);
        else if (num == 2)
        {
          if (diff != nullptr)
          {
            ans = InputAnswer("Input variable name: ");
            size_t num = par.find_var(ans);
            if (num == -1)
              printf("No variables such this\n");
            else
              tex_dump(buf, diff[num]);
          }
        }

        break;
    }
  }
}

#undef TEX_DMP

#undef TEX_END
