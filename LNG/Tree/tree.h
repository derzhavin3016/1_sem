#ifndef __TREE_H_
#define __TREE_H_


#include "..\..\DEFS.h"
#include "..\Node\node.h"
#include "..\dic.h"

#define TREE_LOCATION  __LINE__, __FILE__, __FUNCSIG__

#define TREE_ASSERT(cond, err)           if (!(cond))                               \
                                         {                                          \
                                           throw Error(err, TREE_LOCATION);          \
                                         }

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

  const size_t ANSWER_MAX = 1024;
 

#define DEF_OP(num, name, calc, diff)  OPER_##name = num,

  enum opers
  {
    #include "..\cmd.h"
  };

#undef DEF_OP
 

  class tree
  {
  protected:
    node *root;
    char *buf;
    size_t buf_size;
    FILE *tr;
    stock<string> variables;
    const char *buf_ptr = "";


    // default constructor
    tree( void );

    bool dump( const char filename[], node *nd );

    void rec_dump( node *node ) const;

    bool _print_tree( node *node ) const;

    const char * _get_op_col( char num ) const;

    const char * _get_op_shp( const string &name ) const;

    void _build_tree( node **nd );

    node * _check_buf_ptr( size_t tok_size );

    node_type _check_one_smb( char smb );

    node_type _check_ops( const char *str, size_t size );

    // class destructor
    ~tree( void );
  public:
    void dump_root( const char filename[] )
    {
      dump(filename, root);
    }

    void _read_tree( const char filename[] );
  };
}


#endif // !__TREE_H_

