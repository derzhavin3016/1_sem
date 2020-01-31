#ifndef __DIFF_H_
#define __DIFF_H_

#define STK_OFF_SEC
#include "..\DEFS.h"
#include "Parser/parser.h"
#include <stdarg.h>
#include <time.h>

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
 
  extern const char *phrases[];

  const size_t MAX_BUFF = 100;
  class tree
  {
  private:
    node *root;
    node **diff;
    bool is_diff_init;

    int symp_counter;

    char *buf;
    size_t buf_size;

    parser par;

    unsigned toks_size;
    token* toks;

    stock<double> accuracies;
  public:
    // Default constructor
    tree( void );

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
    ~tree( void );

    node *get_root( void )
    {
      return root;
    }

    node **get_diff( void )
    {
       return diff;
    }

    double laba_kill( const char filename[] ); 

    void tex_dump( const char filename[], node* root );

    void tex_fun_dump( const char filename[] );

    void write_answ( const char filename[], double answ );

  private:
    
    double _getNum( unsigned *pos );

    unsigned _getWord( unsigned *pos );

    void _pre_par( void );

    void _get_acc( void );

    void _vars_init( void );

    void _tree_vars_init( node *start );

    bool tree_diff( size_t var_num );

    void _diff_simpl( void );

    bool _calc_tree( node *nd );

    void _diff_init( void );

    void _op_tex_rec( FILE *f, node *nd );

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

