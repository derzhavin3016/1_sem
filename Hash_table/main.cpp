//#include "Hash_table.h"
#include "hash_func.h"


using namespace ad6;



int main( void )
{
  hash_table tbl;
  const int size_func = 5;

  #define DEF_FNC(FNC) #FNC";",

  const char *names[size_func] = {
                             #include "FNC.h"
                           };

  #undef DEF_FNC
  
 #define DEF_FNC(FNC) FNC,

  hash_t (*Hash[size_func])( const String *str ) =  {
                                                      #include "FNC.h"
                                                    };

  if (!tbl.LoadTxt("dic.txt"))
    return 1;

  for (int i = 0; i < size_func; i++)
  {
    tbl.Hashing(Hash[i]);
    tbl.PutToCSV("lab.csv", ';', names[i]);
    tbl.Clear();
  }
  

  return 0;
}