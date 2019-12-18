#ifndef __BACKEND_H_
#define __BACKEND_H_

#include "..\Tree\tree.h"

#define BE_LOCATION  __LINE__, __FILE__, __FUNCSIG__

#define BE_ASSERT(cond, err)            if (!(cond))                               \
                                         {                                          \
                                           throw Error(err, TREE_LOCATION);          \
                                         }


namespace ad6
{
  class backend : public tree
  {
  private:
    int num_if;
    int num_while;
    int var_cnt;
  public:

    backend( void );



    void tree_to_asm( const char file_in[], const char file_out[] );

    ~backend( void );

  private:

    void _push_globals( void );

    void _rec_print_asm( node *nd );

    void _fprint( const char format[], ... );

    void _asm_op( node *nd );
    
    void _asm_pol_op( node *nd );

    int _change_fnc( string &name );

    void _to_asm( const char filename[] );

    void _asm_cmp( node *nd );

    void _asm_fnc( node *nd );
  };
}

#endif /* __BACKEND_H_ */

