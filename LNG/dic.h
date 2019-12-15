#ifndef __DIC_H_
#define __DIC_H_ 

#include "..\DEFS.h"

namespace ad6
{
  class oper
  {
  private:
    char num;
    const char *name;
    const char *color;
    const char *shp;
  public:

    oper( void );

    oper( const char *s, const char* sh );

    oper( char num, const char* col );

    const char * get_name( void ) const;

    char get_num( void ) const;

    const char * get_color( void ) const;

    const char * get_shape( void ) const;
  };
}


#endif /* _DIC_H_ */

