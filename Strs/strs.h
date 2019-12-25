#ifndef __STRS_H_
#define __STRS_H_

#include <iostream>
#include <cstring>
#include <stdarg.h>
#include "../Differentiator/STOCK.H"

#pragma warning (disable: 4996)

namespace ad6
{
  const unsigned DEF_SIZE = 100;

  class string
  {
  private:
    const  char *str;
    unsigned len;
  public:
    friend int StrCmp( string *str1, string* str2 );
    friend int StriCmp( string *str1, string* str2 );

    friend int StrChrCmp( const  char s[], const string &st );

    friend int StrChrCmp( const  char s[], string &st );

    friend std::ostream & operator<<( std::ostream &out, string &str );

    string( void );

    string( const  char *s, unsigned size );
    
    string( const string &st ) : str(st.str),
                                 len(st.len)
    {
    }

    void print_in_file( FILE *f ) const;

    void print_in_str( char *st ) const;

    const  char *str_ptr( void ) const;

    string &operator=( const string &s );

    friend int StrCmp( string *str1, const string* str2 );

    bool operator==( string &s );

    bool operator==( const string &s );

    unsigned size( void );

     char operator[]( unsigned index ) const;

    

    ~string( void );
  };

  std::ostream & operator<<( std::ostream &out, string &str );

  void StrCpy(  char *Dest, const  char* Source );

  int StrCmp( string *str1, string* str2 );

  int StriCmp( string *str1, string* str2 );

  int StrChrCmp( const  char s[], string &st );
}


#endif /* __STRS_H_ */

