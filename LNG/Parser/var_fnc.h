#ifndef __VAR__FNC_H_
#define __VAR__FNC_H_

#include "..\..\DEFS.h"


namespace ad6
{
  // function container
  class fnc
  {
    string name;
    int args;

  public:

    fnc( void );

    fnc( const string &s, int arg );

    fnc( string &s, int arg );

    explicit fnc( const fnc& f );

    fnc & operator=( const fnc& f );

    bool operator==( const fnc& f );

    const string & get_name( void );

    int get_args( void );

    void set_fnc( const string &s, int arg );

    void set_fnc( string &s, int arg );
  };

  class var
  {
    string name;
    int fnc_num;

  public:

    var( void );

    var( const string &s, int num );

    var( string &s, int num );

    explicit var( const var& f );

    var & operator=( const var& f );

    bool operator==( const var& f );

    string & get_name( void );

    int get_fnc_num( void );

    void set_var( const string &s, int num );

    void set_var( string &s, int num );
  };
}


#endif /* __VAR__FNC_H_ */

