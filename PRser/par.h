#ifndef __PAR_H_
#define __PAR_H_

#include <iostream>
#include <assert.h>

namespace ad6
{
  struct Error
  {
    const char* descr;
    int line;
    const char* file;
    const char* func;
    //Error* reason;

    Error( const char* des, int l, const char* filename, const char* function ) : descr(des),
                                                                                  line(l),
                                                                                  file(filename),
                                                                                  func(function)
    {
    }
  };

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

    int getId( void );

    int getFunc( void );

    ~parser( void )
    {
    }
  };
}

#endif /* __PAR_H_ */
