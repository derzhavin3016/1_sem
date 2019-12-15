#ifndef __LNG_H_
#define __LNG_H_

#define STK_OFF_SEC
#include "..\DEFS.h"
#include "Parser/parser.h"
#include "dic.h"
#include <stdarg.h>

#define TREE_LOCATION  __LINE__, __FILE__, __FUNCSIG__

#define TREE_ASSERT(cond, err)           if (!(cond))                               \
                                         {                                          \
                                           throw Error(err, TREE_LOCATION);          \
                                         }

#pragma warning (disable: 4267)
#pragma warning (disable: 4996)

namespace ad6
{

  const oper smb_op[] = 
  {
   {'+', "gold"},
   {'-', "orchid2"},
   {'*', "pink"},
   {'/', "peachpuff"},
   {'^', "ivory"},
  };

  const oper pol_op[] = 
  {
   {"while", "pentagon"},
   {"if", "hexagon"},
   {"if-else", "hexagon"},
   {"ret", "septagon"}
  };

  char * InputAnswer( const char printfstr[], ... );
  const size_t ANSWER_MAX = 1024;
  
  size_t IsCorrBraces( char *buf, size_t size );

#define DEF_OP(num, name, calc, diff)  OPER_##name = num,

  enum opers
  {
    #include "cmd.h"
  };

#undef DEF_OP
 

  const size_t MAX_BUFF = 100;
  class tree
  {
  private:
    node *root;
    node **diff;
    int symp_counter;
    char *buf;
    size_t buf_size, toks_size;
    parser par;
    bool is_diff_init;
    token* toks;

  public:
    // Default constructor
    tree( void ) : root(),
                   toks(nullptr),
                   toks_size(0),
                   diff(nullptr),
                   buf(nullptr),
                   buf_size(0),
                   symp_counter(0),
                   is_diff_init(false)
    {
    }

    bool save_tree( const char filename[] ) const;

    bool read_tree( const char filename[] );

    bool dump_root( const char filename[] )
    {
      return dump(filename, root);
    }

    // Destructor
    ~tree( void )
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
        delete root;
      if (buf != nullptr)
        delete[] buf;
    }
    node *get_root( void )
    {
      return root;
    }

    node **get_diff( void )
    {
       return diff;
    }


    void tex_dump( const char filename[], node* root );

  private:

    size_t _getWord( size_t *pos );

    double _getNum( size_t *pos );

    void _pre_par( void );

    bool _find_var_tree( node *start, size_t var_num ) const;

    bool dump( const char filename[], node *nd ) const;

    void rec_dump( FILE *dmp, node *node ) const;

    bool _print_tree( FILE *f, node *node ) const;

    int find_op( char sym );

    const char * _get_op_col( char num ) const;

    const char * _get_op_shp( const string &name ) const;
  };

}


#endif /* __LNG_H_ */

