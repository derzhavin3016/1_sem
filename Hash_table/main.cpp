//#include "Hash_table.h"
#include "hash_func.h"


using namespace ad6;

const int MAX_NAME = 100;

int main( void )
{
  hash_table tbl;
  const int size_func = 7;
  char FileName[MAX_NAME] = {};

  #define DEF_FNC(FNC) #FNC";",

  const char *names[size_func] = {
                                   #include "FNC.h"
                                 };


  #undef DEF_FNC
  
  #define DEF_FNC(FNC) FNC,

  hash_t (*Hash[size_func])( const String *str ) =  {
                                                      #include "FNC.h"
                                                    };

  int Ok = Input("##Hash table program\n\n"
                 "Input file name to start: ", "%s", FileName);

  assert(Ok);

  if (!tbl.LoadTxt(FileName))
    return 1;

  Ok = Input("Input file name to save: ", "%s", FileName);
  assert(Ok);

  for (int i = 0; i < size_func; i++)
  {
    tbl.Hashing(Hash[i]);
    tbl.PutToCSV(FileName, ';', names[i]);
    tbl.Clear();
  }


  return 0;
}