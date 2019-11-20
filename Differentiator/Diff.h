#ifndef __DIFF_H_
#define __DIFF_H_

#define STK_OFF_SEC
#include "..\DEFS.h"
#include <stdarg.h>

#define TREE_LOCATION  __LINE__, __FILE__, __FUNCSIG__

#define TREE_ASSERT(cond, err)           if (!(cond))                               \
                                        {                                          \
                                          throw Error{err, TREE_LOCATION};          \
                                        }

#pragma warning (disable: 4267)
#pragma warning (disable: 4996)

namespace ad6
{
  char * InputAnswer( const char printfstr[], ... );
  const size_t ANSWER_MAX = 1024;

  struct Error
  {
    const char* descr;
    int line;
    const char* file;
    const char* func;
    //Error* reason;

    Error( const char* des, int l, const char* filename, const char* function ) : descr(des),
                                                                                  line(l),
                                                                                  file(filename),
                                                                                  func(function)
    {
    }
  };

  
#define DEF_OP(num, name, calc, str)  OPER_##name = num,

  enum opers
  {
    #include "cmd.h"
  };

#undef DEF_OP

  enum node_type
  {
    TYPE_NUMBER = 1,
    TYPE_VAR = 2,
    TYPE_OPERATOR = 3
  };

  struct node
  {
    char *name;
    double value;
    node_type type;
    size_t num;
    node *parent;
    node *right;
    node *left;
    
    // default constructor
    node( void ) : name(nullptr),
                   right(nullptr),
                   left(nullptr),
                   parent(nullptr),
                   value(0),
                   num(0)
    {
    }

    node( node_type tpe, size_t op_num, node *l, node *r ) : type(tpe),
                                                             left(l),
                                                             right(r)
    {
    }

    node &createnode( void )
    {
      return *this;
    }

    ~node( void )
    {
      if (right != nullptr)
      {
        delete right;
        right = nullptr;
      }
      if (left != nullptr)
      {
        delete left;
        left = nullptr;
      }
      if (name != nullptr)
        delete[] name;
      parent = nullptr;
    }
  }; 

  class tree
  {
  private:
    node *root;
    node *diff;
    List<char *> variables;

  public:
    // Default constructor
    tree( void ) : root(),
                   diff(),
                   variables()
    {
    }

    bool save_tree( const char filename[] ) const;

    bool read_tree( const char filename[] );

    void process_loop( void );

    bool dump( const char filename[] ) const;

    double tree_calc( void ) const;

    void tree_diff( void );

    // Destructor
    ~tree( void )
    {
      if (root != nullptr)
        delete root;
      if (diff != nullptr)
        delete diff;

    }
  private:
    
    node & rec_diff( node &nd );

    void rec_dump( FILE *dmp, node *node ) const;

    bool print_tree( FILE *f, node *node ) const;

    bool build_tree( char *buf );

    bool create_leaves( node *prnt );

    double rec_calc( node *nd ) const;

    int find_op( char *string );

    int find_var( char *var );
  };

}


#endif /* __DIFF_H_ */

