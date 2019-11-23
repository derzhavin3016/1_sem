#ifndef __STRS_H_
#define __STRS_H_

#include <iostream>
#include <cstring>
#include <stdarg.h>
#include "../Differentiator/STOCK.H"

#pragma warning (disable: 4996)

namespace ad6
{
  const size_t DEF_SIZE = 100;

  class string
  {
  private:
    const char *str;
    size_t len;
  public:
    friend int StrCmp( string *str1, string* str2 );
    friend int StriCmp( string *str1, string* str2 );

    friend int StrChrCmp( const char s[], string &st );

    friend std::ostream & operator<<( std::ostream &out, string &str );

    string( void );

    string( const char *s, size_t size );
    
    void print_in_file( FILE *f );

    const char *str_ptr( void );

    string &operator=( const string &s );

    bool operator==( string &s );

    size_t size( void );

    char operator[]( size_t index ) const;

    

    ~string( void );
  };

  std::ostream & operator<<( std::ostream &out, string &str );

  void StrCpy( char *Dest, const char* Source );

  int StrCmp( string *str1, string* str2 );

  int StriCmp( string *str1, string* str2 );

  int StrChrCmp( const char s[], string &st );
}


#endif /* __STRS_H_ */

