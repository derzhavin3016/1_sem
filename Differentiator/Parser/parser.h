#ifndef __PARSER_H_
#define __PARSER_H_

#include "..\..\DEFS.h"
#include "..\Node\node.h"

namespace ad6
{
  class parser
  {
  private: 
      const char *ptr = "";
      stock<string> variables;
  public:

    // default constructor
    parser( void ) : variables()
    {
    }

    // destructor
    ~parser( void )
    {
    }

    size_t var_size( void )
    {
      return variables.size();
    }

    node * getG( const char *str );

    int find_var( const char str[] );

  private:
    node * _getE( void );
    
    node * _getT( void );
    
    node * _getP( void );
    
    node * _getN( void );
    
    node * _getId( void );
    
    node * _getFunc( void );

    node * _getPow( void );
  };
}

#endif /* __PARSER_H_ */

