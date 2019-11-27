#ifndef __DIFF_H_
#define __DIFF_H_

#define STK_OFF_SEC
#include "..\DEFS.h"
#include "Parser/parser.h"
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
    size_t buf_size;
    parser par;
    bool is_diff_init;
  public:
    // Default constructor
    tree( void ) : root(),
                   diff(nullptr),
                   buf(nullptr),
                   buf_size(0),
                   symp_counter(0),
                   is_diff_init(false)
    {
    }

    bool save_tree( const char filename[] ) const;

    bool read_tree( const char filename[] );

    void process_loop( void );

    bool dump_root( const char filename[] )
    {
      return dump(filename, root);
    }

    double tree_calc( void ) const;

    bool tree_diff( const char var[] );

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

    void _diff_init( void );

    bool _simplifier( node **nd );

    void _fill_diff( void );

    bool _find_var_tree( node *start, size_t var_num ) const;

    void _tex_rec( FILE *f, node *nd );

    node* _rec_symp( node *nd );

    bool _is_calc( node *nd ) const;

    bool dump( const char filename[], node *nd ) const;

    node & rec_diff( node &nd, size_t var_num );

    void rec_dump( FILE *dmp, node *node ) const;

    bool print_tree( FILE *f, node *node ) const;

    bool build_tree( char *buf );

    double rec_calc( node *nd ) const;

    int find_op( char sym );

    int find_var( char *var );
  };

}


#endif /* __DIFF_H_ */

