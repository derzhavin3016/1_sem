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
  const unsigned MAX_BUFF = 100;
  class frontend : public base_translator
  {
  private:
    unsigned toks_size;
    parser par;
    token* toks;

  public:
    // Default constructor
    frontend( void ) : base_translator(),
                       toks(nullptr),
                       toks_size(0)
    {
    }

    virtual void translate( const char file_in[], const char file_out[] ) override;

    // Destructor
    ~frontend( void )
    {
      if (toks != nullptr)
        delete[] toks;
    }

  private:

    void _par_tree( const char file_in[], const char file_out[] );

    bool _read_par_tree( const char filename[] );

    unsigned _getWord( unsigned *pos );

    double _getNum( unsigned *pos );

    void _pre_par( void );

    bool _find_var_tree( node *start, unsigned var_num ) const;

    int find_op( char sym );
  };

}


#endif /* __LNG_H_ */

