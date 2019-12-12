#include "Tok.h"


ad6::token::token( void ) : num(0),
                            name(),
                            value(0)
                          
{
}

ad6::token::token( double value, int ln, int pos ) : type(TOK_NUM),
                                                     value(value),
                                                     line(ln),
                                                     pos(pos)
{
}

ad6::token::token( tok_type tpe, size_t number, int ln, int p,
                   const char *var_name, size_t name_size ) : type(tpe),
                                                              num(number),
                                                              line(ln),
                                                              pos(p),
                                                              name(var_name, name_size)
{
}

ad6::token::token( const token &tok ) : type(tok.type),
                                        num(tok.num),
                                        line(tok.line),
                                        pos(tok.pos),
                                        name(tok.name),
                                        value(tok.value)
{
}

ad6::token & ad6::token::operator=( const token &tok )
{
  type = tok.type;
  num = tok.num;
  line = tok.line;
  pos = tok.pos;
  name = tok.name;
  value = tok.value;

  return *this;
}

ad6::tok_type ad6::token::get_type( void ) const
{
  return type;
}

double ad6::token::get_value( void ) const
{
  return value;
}

size_t ad6::token::get_num( void ) const
{
  return num;
}

void ad6::token::set_type( tok_type tpe )
{
  type = tpe;
}

void ad6::token::set_value( double val )
{
  value = val;
}

void ad6::token::set_num( size_t number )
{
  num = number;
}

ad6::token::~token( void )
{
}
