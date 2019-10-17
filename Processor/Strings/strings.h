#ifndef __STRINGS_H_
#define __STRINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define IfEndStr(A) (A != '\0' && A != '\n')


#pragma warning (disable: 4996 4090)

typedef struct
{
  const char *str;
  size_t len;
} String;

char * LoadAndCreateStrings( const char filename[], size_t *str_count, int *error_code );

int StrCount( const char buf[], size_t buf_size, char end_value );

String *CreateStringsPtrs( const char buf[], size_t buf_size, size_t *str_cnt, char end_value );

int FileLength( FILE *f );

void LACS_Process_Error( int error_code );

int StrCompareBegin( const char s1[], const char s2[] );

void PrintStrs( const char filename[], const String strs[], size_t size );

void DelCom( String *strs, size_t size, char end_value );

void StrConcat( char Dest[], const char Src[] );

bool PutBufToFile( const char file_name[], const char buf[], size_t buf_size );

enum
{
  LACS_LENGTH_ERROR = 0,
  LACS_FSEEK_ERROR,        //1
  LACS_BUF_MEMORY_ERROR,   //2
  LACS_FOPEN_ERROR         //3
};


#endif /* __STRINGS_H_ */
