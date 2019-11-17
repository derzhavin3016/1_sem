#ifndef __STRINGS_H_
#define __STRINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define IfEndStr(A) (A != '\0' && A != '\n')


#pragma warning (disable: 4996 4090)

struct String
{
  const char *str;
  size_t len;

  bool operator==( const String* s );
};

char * LoadAndCreateStrings( const char filename[], size_t *str_count, int *error_code );

int StrCount( const char buf[], size_t buf_size, char end_value );

String *CreateStringsPtrs( const char buf[], size_t buf_size, size_t *str_cnt, char end_value );

int FileLength( FILE *f );

void LACS_Process_Error( int error_code );

int StrCompareBegin( const char s1[], const char s2[], char end_sym = '\0');

void PrintStrs( const char filename[], const String strs[], size_t size );

void DelCom( String *strs, size_t size, char end_value );

void StrConcat( char Dest[], const char Src[] );

bool PutBufToFile( const char file_name[], const char buf[], size_t buf_size );

char* FillBuf( const char file_in[], size_t *code_size );

int StrCompareBegin( char s1[], char s2[] );

enum
{
  LACS_LENGTH_ERROR = 0,
  LACS_FSEEK_ERROR,        //1
  LACS_BUF_MEMORY_ERROR,   //2
  LACS_FOPEN_ERROR         //3
};


#endif /* __STRINGS_H_ */
