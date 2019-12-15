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
  private:
    stock<string> variables;
    const char *buf_ptr = "";
  public:
    // class constructor
    fback( void );

    void dump_root( const char filename[] )
    {
      dump(filename, root);
    }

    void read_tree( const char filename[] );

    // class destructor
    ~fback( void );
    private:

    void _build_tree( node **nd );

    node * _check_buf_ptr( size_t tok_size );

    node_type _check_one_smb( char smb );

    node_type _check_ops( const char *str, size_t size );
  };
}

#endif /* __FRONTBACK_H_ */

