#ifndef __LNG_H_
#define __LNG_H_

#define STK_OFF_SEC
#include "..\Tree\tree.h"
#include "..\Parser/parser.h"
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
  const size_t MAX_BUFF = 100;
  class frontend : public tree
  {
  private:
    size_t toks_size;
    parser par;
    token* toks;

  public:
    // Default constructor
    frontend( void ) : tree(),
                       toks(nullptr),
                       toks_size(0)
    {
    }

    void par_tree( const char file_in[], const char file_out[] );

    // Destructor
    ~frontend( void )
    {
    }

  private:

    bool _read_par_tree( const char filename[] );

    bool _save_tree( const char filename[] );

    size_t _getWord( size_t *pos );

    double _getNum( size_t *pos );

    void _pre_par( void );

    bool _find_var_tree( node *start, size_t var_num ) const;

    int find_op( char sym );
  };

}


#endif /* __LNG_H_ */

