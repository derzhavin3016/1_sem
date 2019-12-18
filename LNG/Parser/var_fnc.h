#ifndef __VAR__FNC_H_
#define __VAR__FNC_H_

#include "..\..\DEFS.h"


namespace ad6
{
  const int GLOBAL_VAR = -1;

  class dic_var;

  class var;
  // function container
  class fnc
  {
    string name;
    int args;
    stock<dic_var> table;

  public:

    int var_nums( void )
    {
      return table.size() - args;
    }

    friend void FillFncTables( stock<fnc> &funcs, stock<var> &vars );

    fnc( void );

    fnc( string &name );

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
    bool IsInit;
    int offset;
  public:

    friend void FillFncTables( stock<fnc> &funcs, stock<var> &vars );

    var( void );

    void set_offset( int of )
    {
      offset = of;
    }

    int get_offset( void )
    {
      return offset;
    }

    var( const string &s, int num );

    var( string &s, int num );

    explicit var( const var& f );

    var & operator=( const var& f );

    bool operator==( const var& f );

    string & get_name( void );

    bool Is_Init( void ) const;

    void Init( void );

    int get_fnc_num( void );

    void set_var( const string &s, int num );

    void set_var( string &s, int num );
  };


  class dic_var
  {
  private:
    int offset;
    string name;

  public:

    int get_offset( void )
    {
      return offset;
    }

    dic_var( void );

    dic_var( const string &s, int of );

    dic_var( string &s, int of );

    dic_var & operator=( const dic_var& f );

    bool operator==( const dic_var& f );
  };

  void FillFncTables( stock<fnc> &funcs, stock<var> &vars );
}


#endif /* __VAR__FNC_H_ */

