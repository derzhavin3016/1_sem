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

ad6::fnc::fnc( string &name ) : name(name),
                                args(-1)
{
}

// overloaded comprasion operator
bool ad6::fnc::operator==( const fnc& f )
{
  if (name == f.name && (args == f.args || f.args == -1))
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
                                            fnc_num(num),
                                            IsInit(false)
{
}

ad6::var::var( string &s, int num ) : name(s),
                                      fnc_num(num),
                                      IsInit(false)
{
}

ad6::var::var( const var& f ) : name(f.name),
                                fnc_num(f.fnc_num),
                                IsInit(false)
{
}

ad6::var & ad6::var::operator=( const var& f )
{
  name = f.name;
  fnc_num = f.fnc_num;
  IsInit = f.IsInit;

  return *this;
} 

bool ad6::var::Is_Init( void ) const
{
  return IsInit;
}

void ad6::var::Init( void )
{
  IsInit = true;
}

bool ad6::var::operator==( const var& f )
{
  if (!IsInit && name == f.name && (fnc_num == f.fnc_num || f.fnc_num == GLOBAL_VAR || fnc_num == GLOBAL_VAR))
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


ad6::dic_var::dic_var( void ) : name(),
                                offset(0)
{
}

ad6::dic_var::dic_var( const string &s, int of ) : name(s),
                                                   offset(of)
{
}

ad6::dic_var::dic_var( string &s, int of ) : name(s),
                                             offset(of)
{

}


ad6::dic_var & ad6::dic_var::operator=( const dic_var& f )
{
  name = f.name;
  offset = f.offset;


  return *this;
}

bool ad6::dic_var::operator==( const dic_var& f )
{
  return (name == f.name && offset == f.offset);
}

void ad6::FillFncTables( stock<fnc> &funcs, stock<var> &vars )
{
  for (unsigned  i = 0; i < vars.size(); i++)
  {
    int num = 0;
     if ((num = vars[i].fnc_num) != GLOBAL_VAR)
    {
      int size = funcs[num].table.size();
      funcs[num].table.add(dic_var(vars[i].name, size - funcs[num].args));
      vars[i].offset = size - funcs[num].args;
    }
  } 
}
