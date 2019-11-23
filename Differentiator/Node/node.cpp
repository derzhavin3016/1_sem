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

// nunber constructor
ad6::node::node( double value ) : type(TYPE_NUMBER),
                                  value(value),
                                  left(nullptr),
                                  right(nullptr)
{
}


ad6::node::node( const char *var_name, size_t name_size, size_t var_num ) : type(TYPE_VAR),
                                                                            name(var_name, name_size),
                                                                            num(var_num),
                                                                            left(nullptr),
                                                                            right(nullptr)
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