#ifndef __TOK_H_
#define __TOK_H_

#include "..\Node\node.h"

namespace ad6
{
  enum tok_type
  {
    TOK_NUL = 0,
    TOK_NUM = 1,
    TOK_STR = 2,
    TOK_SMB = 3
  };

  class token
  {
  private:
    string name;
    tok_type type;
    double value;
    size_t num;
    int line, pos;
  public:

    // default constructor
    token( void );

    // number constructor
    token( double value );

    token( const token &tok );

    // copy constructor
    token( double value, int ln, int pos );

    token( tok_type tpe, size_t number, int ln, int p, const char *var_name = nullptr, size_t name_size = 0 );

    token & operator=( const token &tok );

    tok_type get_type( void ) const;

    double get_value( void ) const;

    const char *get_str( void ) const
    {
      return name.str_ptr();
    }
    const string & get_string( void ) const
    {
      return name;
    }

    int get_pos( void ) const
    {
      return pos;
    }

    int get_line( void ) const
    {
      return line;
    }

    size_t get_num( void ) const;

    void set_type( tok_type tpe );

    void set_value( double val );

    void set_num( size_t number );

    ~token( void );
  };
}


#endif /* __TOK_H_ */

