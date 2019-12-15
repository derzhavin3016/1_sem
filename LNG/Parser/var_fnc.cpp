#include "var_fnc.h"

// function class default constructor
ad6::fnc::fnc( void ) : name(),
              args(0)
{
}

// function class constructor by constant link to string and amount of arguments
ad6::fnc::fnc( const string &s, int arg ) : name(s),
                                            args(arg)
{
}

// function class constructor by link to string and amount of arguments
ad6::fnc::fnc( string &s, int arg ) : name(s),
                                      args(arg)
{
}

// copy constructor
ad6::fnc::fnc( const fnc& f ) : name(f.name),
                                args(f.args)
{
}

// overloaded operator '=' (copy constructor)
ad6::fnc & ad6::fnc::operator=( const fnc& f )
{
  name = f.name;
  args = f.args;

  return *this;
}

// overloaded comprasion operator
bool ad6::fnc::operator==( const fnc& f )
{
  if (name == f.name && args == f.args)
    return true;
  return false;
}

// get refernece to name fucntion
const ad6::string & ad6::fnc::get_name( void )
{
  return name;
}

// get amount of arguments
int ad6::fnc::get_args( void )
{
  return args;
}

// set data fucntion
void ad6::fnc::set_fnc( const string &s, int arg )
{
  name = s;
  args = arg;
}

// set data function (with unconstant reference to string)
void ad6::fnc::set_fnc( string &s, int arg )
{
  name = s;
  args = arg;
}

// default constructor
ad6::var::var( void ) : name(),
                        fnc_num(0)
{
}

ad6::var::var( const string &s, int num ) : name(s),
                                            fnc_num(num)
{
}

ad6::var::var( string &s, int num ) : name(s),
                                      fnc_num(num)
{
}

ad6::var::var( const var& f ) : name(f.name),
                                fnc_num(f.fnc_num)
{
}

ad6::var & ad6::var::operator=( const var& f )
{
  name = f.name;
  fnc_num = f.fnc_num;

  return *this;
} 

bool ad6::var::operator==( const var& f )
{
  if (name == f.name && fnc_num == f.fnc_num)
    return true;

  return false;
}

ad6::string & ad6::var::get_name( void )
{
  return name;
}

int ad6::var::get_fnc_num( void )
{
  return fnc_num;
}

void ad6::var::set_var( const string &s, int num )
{
  name = s;
  fnc_num = num;
}

void ad6::var::set_var( string &s, int num )
{
  name = s;
  fnc_num = num;
}

