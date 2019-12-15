#include "Lng.h"

#define TEX_DMP \
"\\documentclass[12pt,a4paper,fleqn]{article} \n"\
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

void ad6::frontend::_pre_par( void )
{
  toks = new token[buf_size + 1];
  size_t str_counter = 0, pos_counter = 0;
  int pos = 0;

  for (size_t i = 0; i < buf_size; i++)
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
      size_t end_word = _getWord(&i);

      toks[toks_size++] = token(TOK_STR, 0, str_counter + 1, pos_counter + 1, buf + i, end_word);
      i += end_word - 1;
      continue;
    }
    toks[toks_size++] = token(TOK_SMB, 0, str_counter + 1, pos_counter + 1, buf + i, 1);
  }
  toks[toks_size++] = token(TOK_NUL, 0, str_counter + 1, buf_size);
}

/*
*/
size_t ad6::frontend::_getWord( size_t *pos )
{
  size_t new_pos = 0;
  
  do
  {
    new_pos++;
  } while (isalpha(buf[*pos + new_pos]));

  return new_pos;
} /* End of '_getWord' function */

/* 
*/
double ad6::frontend::_getNum( size_t *pos )
{
  double num = 0;
  size_t add_pos = 0;
  
  if (sscanf(buf + *pos, "%lg%n", &num, &add_pos) != 1)
    return NAN;

  *pos = *pos + add_pos - 1;
  return num;
} /* End of '_getNum' function */

/**
 * \brief Read tree from file function.
 * \param [in] pointer to file structure
 * \param [in]  filename  Name of a file to read from.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::frontend::read_tree( const char filename[] )
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


/**
 * \brief Save tree to file function.
 * \param [in]  filename  Name of a file to save.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::frontend::save_tree( const char filename[] ) const
{
  TREE_ASSERT(filename != nullptr, "Incorrect file name");

  FILE *tree = fopen(filename, "w");

  if (tree == nullptr)
  {
    printf("Oh, some errors with openning file \"%s\"", filename);
    return false;
  }

  _print_tree(tree, root);

  fclose(tree);
  return true;
} /* End of 'save_tree' function */


/**
 * \brief Find operator number function.
 * \brief string ppointer to string.
 * \return number of the operator if it was find.
 * \return -1 otherwise.
*/
int ad6::frontend::find_op( char sym )
{

    #define DEF_OP(num, name, calc, diff)                 \
     if (sym == num)                              \
       return num;

  #include "..\cmd.h"

  #undef DEF_OP
  return -1;
} /* End of 'find_op' function */

/**
 * \brief Find variable in tree function.
 * \param [in] start Pointer to tree's node.
 * \param [in] var_num Number of variable in variables array.
*/
bool ad6::frontend::_find_var_tree( node *start, size_t var_num ) const
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


#undef TEX_DMP

#undef TEX_END  
