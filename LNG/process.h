#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "Frontend\Lng.h"
#include "Frontend^-1/frontback.h"
#include "Backend/Backend.h"
#include "Mid/mid.h"

#define GREETING  "Hello. I'm an artificial intelligence.\n"                              \
                  "My dad is Andrey Derzhavin, and he has his own group in vk\n"          \
                  "Subscribe, if you want to know more about my dad's phystech life\n"    \
                  "https://vk.com/andryha_mipt \n\n"                                      \
                  "*** Here what can I do: \n"                                            \
                  "*** 0. Exit (bye bye)\n"                                               \
                  "*** 1. Frontend \n"                                                    \
                  "*** 2. Frontend -1\n"                                                  \
                  "*** 3. Backend\n"                                                      \
                  "*** 4. Middleend\n"                                                    \
                  "*** 5. Dump tree with dot\n"       

#define PARTING "Have a nice night, stranger!"

void MainProcLoop( void );

using namespace ad6;

#endif // !__PROCESS_H_
