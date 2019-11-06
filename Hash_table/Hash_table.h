#ifndef __HASH_TABLE_H_
#define __HASH_TABLE_H_

#include "..\List\List.h"
#include "..\Processor\Strings\strings.h"

typedef unsigned long long hash_t;

namespace ad6
{
  const size_t TABLE_SIZE = 1000;

  class hash_table
  {
  private:
    List<const String *> table[TABLE_SIZE];
    // pointer to hash function
    hash_t (*Hash_func)( const String* str );
    char *buf;
    String *strs;
    size_t buf_size, strs_size;
    bool IsHeaderPrint;
  public:

    // default constructor
    hash_table( void ) : table{},
                         buf(nullptr),
                         buf_size(0),
                         strs_size(0),
                         strs(nullptr),
                         IsHeaderPrint(false)
    {
    }

    // default constructor
    hash_table( hash_t (*func)( const String* s) ) : table{},
                                                     buf(nullptr),
                                                     buf_size(0),
                                                     strs_size(0),
                                                     strs(nullptr),
                                                     IsHeaderPrint(false)
    {
      Hash_func = func;
    }

    bool LoadTxt( const char filename[] );
    
    void Hashing( hash_t (*Hash)( const String *str ), bool IsDic = true );

    bool PutToCSV( const char file_out[], char sep_sym, const char name[] );

    bool Insert( const String *str, bool IsDic = true );

    ~hash_table( void )
    {
      if (buf != nullptr)
        free(buf);
      if (strs != nullptr)
        free(strs);
    }

    void Clear( void );
  private:

    void PrintCSVHeader( FILE* f, char sep_sym );
  };
}

#endif /* __HASH_TABLE_H_ */

