#ifndef __FRONTBACK_H_  
#define __FRONTBACK_H_

#include "..\Tree\tree.h"

#define FB_LOCATION  __LINE__, __FILE__, __FUNCSIG__

#define FB_ASSERT(cond, err)            if (!(cond))                               \
                                         {                                          \
                                           throw Error(err, TREE_LOCATION);          \
                                         }

namespace ad6
{
  class fback : public tree
  {
  public:
    // class constructor
    fback( void );

    void tree_to_code( const char file_in[], const char file_out[] );

    // class destructor
    ~fback( void );
  private:

    void _tree_to_txt( const char filename[] );

    void _print_arif_op( node *nd );

    void _print_pol_op( node *nd, int ind_size );

    void _print_sep( node *nd, int ind_size );

    void _print_spaces( int spc_amount = 1 );

    bool _check_add_var( string &name );

    void _rec_print( node *nd, int ind_size );
  };
}

#endif /* __FRONTBACK_H_ */

