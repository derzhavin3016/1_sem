#ifndef __NODE_H_
#define __NODE_H_

#include "..\..\Strs\strs.h"

namespace ad6
{
  enum node_type
  {
    TYPE_NUMBER = 1,
    TYPE_VAR = 2,
    TYPE_OPERATOR = 3,
    TYPE_FUNC = 4,
    TYPE_SEP = 5,
    TYPE_USR_FNC = 6,
    TYPE_CMP = 7,
    TYPE_POL_OP = 8
  };

  class node
  {
  public:
    string name;
    double value;
    char num;
    bool IsOp;
    node_type type;
    node *right;
    node *left; 

    // default constructor
    node( void );

    // Copy constructor
    explicit node( node& nd );

    // op constructor
    node( node *left, node *right );

    // operator constructor
    node( char op_num, node *l, node *r );

    // separator constructor
    node( bool IsOp, node *l, node *r );

    // variable or function constructor
    node( node_type tpe, const char *var_name, size_t name_size, size_t var_num, node* left = nullptr, node* right = nullptr );

    // number constructor
    node( double value );

    node( node_type tpe, string &str, size_t var_num, node* left = nullptr, node* right = nullptr );

    node( node_type tpe, const string &str, size_t var_num, node* left = nullptr, node* right = nullptr );

    // Copy constructor
    void set_node( node& nd );

    // op constructor
    void set_node( node *l, node *r );

    // operator constructor
    void set_node( char op_num, node *l, node *r );

    // separator constructor
    void set_node( bool Is, node *l, node *r );

    // variable or function constructor
    void set_node( node_type tpe, const char *var_name, size_t name_size, size_t var_num, node* l = nullptr, node* r = nullptr );

    // number constructor
    void set_node( double val );

    void set_node( node_type tpe, string &str, size_t var_num, node* l = nullptr, node* r = nullptr );

    void set_node( node_type tpe, const string &str, size_t var_num, node* l = nullptr, node* r = nullptr );

    ~node( void );
  };
  
  node & operator+( node &n1, node &n2 );

  node & operator*( node &n1, node &n2 );

  node & operator-( node &n1, node &n2 );

  node & operator/( node &n1, node &n2 );

  node & operator^( node &n1, node &n2 );

  node * rec_copy( node *nd );

  node * node_copy( node *root, bool IsDel = false );
}

#endif /* __NODE_H_ */

