#ifndef __PAR_H_
#define __PAR_H_

#include <iostream>
#include <assert.h>

namespace ad6
{
  class parser
  {
  private: 
      const char *ptr = "";
   
  public:

    // default constructor
    parser( void )
    {
    }

    parser( const char *str )
    {
      printf("%d\n", getG(str));
    }

    int getG( const char *str );

    int getE( void );

    int getT( void );

    int getP( void );

    int getN( void );

    ~parser( void )
    {
    }
  };
}

#endif /* __PAR_H_ */
