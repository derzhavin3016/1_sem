#include "node.h"

// default constructor
ad6::node::node( void ) : name(),
                          right(nullptr),
                          left(nullptr),
                          value(0)
{
}

// Copy constructor
ad6::node::node( node& nd ) : name(nd.name),
                              num(nd.num),
                              value(nd.value),
                              type(nd.type),
                              left(nd.left),
                              right(nd.right)
{
}

// operator constructor
ad6::node::node( char op_num, node *l, node *r ) : type(TYPE_OPERATOR),
                                                   num(op_num),
                                                   left(l),
                                                   right(r)
{
}

ad6::node::node( bool IsOp, node *l, node *r ) : type(TYPE_SEP),
                                                 IsOp(IsOp),
                                                 left(l),
                                                 right(r)
{
}

// number constructor
ad6::node::node( double value ) : type(TYPE_NUMBER),
                                  value(value),
                                  left(nullptr),
                                  right(nullptr)
{
}


ad6::node::node( node_type tpe, const char *var_name, size_t name_size, size_t var_num, node* left, node* right ) : type(tpe),
                                                                                                                    name(var_name, name_size),
                                                                                                                    num((char)var_num),
                                                                                                                    left(left),
                                                                                                                    right(right)
{
}
// 


// op cinstructor
ad6::node::node( node *left, node *right ) : type(TYPE_SEP),
                                             name("op", 2),
                                             left(left),
                                             right(right)
{
}

ad6::node::node( node_type tpe, string &str, size_t var_num, node* left, node* right ) : type(tpe),
                                                                                         name(str),
                                                                                         num((char)var_num),
                                                                                         left(left),
                                                                                         right(right)
{
}

ad6::node::node( node_type tpe, const string &str, size_t var_num, node* left, node* right ) : type(tpe),
                                                                                               name(str),
                                                                                               num((char)var_num),
                                                                                               left(left),
                                                                                               right(right)
{
}

ad6::node::~node( void )
{
   if (left != nullptr)
  {
    delete left;
    left = nullptr;
  }
  if (right != nullptr)  
  {
    delete right;
    right = nullptr;
  }
}

ad6::node & ad6::operator+( ad6::node &n1, ad6::node &n2 )
{
  using namespace ad6;

  // ((x+8)-9)
  return *(new node('+', &n1, &n2));
} /* End of 'operator+' function */

ad6::node & ad6::operator*( ad6::node &n1, ad6::node &n2 )
{
  // ((x+8)-9)
  return *(new ad6::node('*', &n1, &n2));
} /* End of 'operator*' function */

ad6::node & ad6::operator-( ad6::node &n1, ad6::node &n2 )
{
  // ((x+8)-9)
  return *(new ad6::node('-', &n1, &n2));
} /* End of 'operator+' function */

ad6::node & ad6::operator/( ad6::node &n1, ad6::node &n2 )
{
  using namespace ad6;
  // ((x+8)-9)
  return *(new ad6::node('/', &n1, &n2));
} /* End of 'operator*' function */

ad6::node & ad6::operator^( ad6::node &n1, ad6::node &n2 )
{
  using namespace ad6;
  // ((x+8)-9)
  return *(new ad6::node('^', &n1, &n2));
} /* End of 'operator*' function */

/**
 * \brief Copy of tree fucntion.
 * \param [in] pointer to tree root.
 * \param [in] IsDel if delete of root needed (false default);
 * \return pointer to new tree's root.
*/
ad6::node * ad6::node_copy( node *root, bool IsDel )
{
  node *new_root = rec_copy(root);

  if (IsDel)
    delete root;

  return new_root;
} /* End of 'node_copy' function */

/**
 * \brief Recursive copy of tree fucntion.
 * \param [in] pointer to tree node.
 * \param [in] IsDel if delete of root needed (false default);
 * \return pointer to new tree's root.
*/
ad6::node * ad6::rec_copy( node *nd )
{
  node *new_nd = new node(*nd);
  new_nd->left = nullptr; 
  new_nd->right = nullptr;

  if (nd->left != nullptr)
    new_nd->left = rec_copy(nd->left);

  if (nd->right != nullptr)
    new_nd->right = rec_copy(nd->right);

  return new_nd;
} /* End of 'rec_copy' function */

// Copy constructor
void ad6::node::set_node( node& nd )
{
  value = nd.value;
  name = nd.name;
  IsOp = nd.IsOp;
  num = nd.num;
  type = nd.type;
  right = nd.right;
  left = nd.left;
}

// op constructor
void ad6::node::set_node( node *l, node *r )
{
  name = string("op", 2);
  type = TYPE_SEP;
  right = r;
  left = l;
}

// operator constructor
void ad6::node::set_node( char op_num, node *l, node *r )
{
  num = op_num;
  type = TYPE_OPERATOR;
  right = r;
  left = l;
}

// separator constructor
void ad6::node::set_node( bool Is, node *l, node *r )
{
  IsOp = Is;
  type = TYPE_SEP;
  right = r;
  left = l;
}

// variable or function constructor
void ad6::node::set_node( node_type tpe, const char *var_name, size_t name_size, size_t var_num, node* l /*= nullptr*/, node* r /*= nullptr*/ )
{
  name = string(var_name, name_size);
  num = (char)var_num;
  type = tpe;
  right = r;
  left = l;
}

// number constructor
void ad6::node::set_node( double val )
{
  value = val;
  type = TYPE_NUMBER;
  right = nullptr;
  left = nullptr;
}

void ad6::node::set_node( node_type tpe, string &str, size_t var_num, node* l /* = nullptr*/, node* r /*= nullptr*/ )
{
  name = str;
  num = (char)var_num;
  type = tpe;
  right = r;
  left = l;
}

void ad6::node::set_node( node_type tpe, const string &str, size_t var_num, node* l /*= nullptr*/, node* r /*= nullptr*/ )
{
  name = str;
  num = (char)var_num;
  type = tpe;
  right = r;
  left = l;
}