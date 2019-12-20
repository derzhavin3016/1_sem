#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "..\Assembler\asm.h"
#include "..\Disassembler\disasm.h"
#include "..\Proc\processor.h"
#include <stdarg.h>
#define NDEBUG

using namespace ad6;

#define CPU_LOCATION __LINE__, __FILE__, __FUNCSIG__

#define CPU_ASSERT(cond, str)  if (!(cond))                      \
                               {                                 \
                                 throw Error{str, CPU_LOCATION}; \
                               }

#define GREETING  "Hello. I'm an artificial intelligence.\n"                              \
                  "My dad is Andrey Derzhavin, and he has his own group in vk\n"          \
                  "Subscribe, if you want to know more about my dad's phystech life\n"    \
                  "https://vk.com/andryha_mipt \n\n"                                      \
                  "*** Here what can I do: \n"                                            \
                  "*** 0. Exit (bye bye)\n"                                               \
                  "*** 1. Assembly   \n"                                                  \
                  "*** 2. Disassembly\n"                                                  \
                  "*** 3. Execute\n"                          

#define PARTING  "Have a nice night, stranger!\n"


void ProcessLoop( void );

char * InputAnswer( const char printfstr[], ... );



struct Error : public std::runtime_error
{
  const char* descr;
  int line;
  const char* file;
  const char* func;
  //Error* reason;

  Error( const char* des, int l, const char* filename, const char* function ) : descr(des),
                                                                                line(l),
                                                                                file(filename),
                                                                                func(function),
                                                                                std::runtime_error(des)
  {
  }

  virtual const std::string what() 
  { 
    return descr; 
  }
};

#endif /* __PROCESS_H_ */

