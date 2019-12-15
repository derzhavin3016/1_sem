#include "dic.h"

ad6::oper::oper( void ) : name(),
                          color(nullptr),
                          shp(nullptr),
                          num(0)
{
}

ad6::oper::oper( const char *s, const char* sh ) : name(s),
                                                     shp(sh)
{
}

ad6::oper::oper( char num, const char* col ) : num(num),
                                               color(col)
{
}

const char * ad6::oper::get_name( void ) const
{
  return name;
}

char ad6::oper::get_num( void ) const
{
  return num;
}

const char * ad6::oper::get_color( void ) const
{
  return color;
}

const char * ad6::oper::get_shape( void ) const
{
  return shp;
}