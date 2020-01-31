#ifndef __PARSER_H_
#define __PARSER_H_

#include "..\..\DEFS.h"
#include "..\Tokens\Tok.h"

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
      const token *ptr;
      stock<var> variables;
  public:

    // default constructor
    parser( void ) : variables()
    {
    }

    const var & operator[]( size_t Index ) const
    {
      return variables[(int)Index];
    }

    var & operator[]( size_t Index )
    {
      return variables[(int)Index];
    }

    // destructor
    ~parser( void )
    {
    }

    size_t var_size( void )
    {
      return variables.size();
    }

    node * getG( const token *str );

    int find_var( const char str[] );

    string & get_var( size_t num )
    {
      return variables[(int)num].name;
    }

  private:
    node * _getE( void );
    
    node * _getT( void );
    
    node * _getP( void );
    
    node * _getN( void );
    
    node * _getId( void );
    
    node * _getFunc( void );

    node * _getPow( void );

    bool _check_ptr_type( tok_type check );
  };
}

#endif /* __PARSER_H_ */

