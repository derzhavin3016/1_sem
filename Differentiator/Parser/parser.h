#ifndef __PARSER_H_
#define __PARSER_H_

#include "..\..\DEFS.h"
#include "..\Node\node.h"

namespace ad6
{
  struct var
  {
    string name;
    double value;

    var( void );

    var( string &st );

    var( const var &v );

    var( string &st, double val );

    var & operator=( const var &v );

    bool operator==( const var &v );
  };

  class parser
  {
  private: 
      const char *ptr = "";
      stock<var> variables;
  public:

    // default constructor
    parser( void ) : variables()
    {
    }

    const var & operator[]( size_t Index ) const
    {
      return variables[Index];
    }

    var & operator[]( size_t Index )
    {
      return variables[Index];
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

    string & get_var( size_t num )
    {
      return variables[num].name;
    }

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

