#ifndef __ONEGIN_H_
#define __ONEGIN_H_

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

int StrCompareBegin( const String *s1, const String *s2 );

void QuickSort( String *str, int first, int last, int (*Comp)( const String *s1, const String *s2 ) );

void Swap( String *a, String *b );

int StrCount( const char buf[], size_t buf_size, char end_value );

String *CreateStringsPtrs( const char buf[], size_t buf_size, size_t *str_cnt, char end_value );

void PrintStrs( const char filename[], const String strs[], size_t size );

int FileLength( FILE *f );

void LACS_Process_Error( int error_code );

int StrCompareEnd( const String *s1, const String *s2 );

__inline size_t Min( size_t a, size_t b );

void PrintStr( const char filename[], const char str[] );

void StrSeparate( const char file_name[], int height, int width, char sep_sym );

/**
 * Input one value template fucntion.
 * \param [in]  promt     Promt string to print.
 * \param [in]  scanfstr  String for scanf fucntion (with expected value).
 * \param [out] value     Pointer to value to input.
 * \return 1 if all is OK, 0 otherwise.
 */
template <typename T>
int Input( const char promt[], const char scanfstr[], T *value )
{
  if (promt == NULL || scanfstr == NULL || value == NULL)
    return 0;

  printf(promt);
  return scanf(scanfstr, value);
}

enum
{
  LACS_LENGTH_ERROR = 0,
  LACS_FSEEK_ERROR,        //1
  LACS_BUF_MEMORY_ERROR,   //2
  LACS_FOPEN_ERROR         //3
};


#endif /* __ONEGIN_H_ */
