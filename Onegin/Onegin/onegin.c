#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define MAX_NAME 1000

#define IfEndStr(A) (A != '\0' && A != '\n')

#pragma warning (disable: 4996 4090)

typedef struct
{
  char *str;
  int len;
} String;

char *LoadAndCreateStrings( const char filename[] );

int StrCompare( const char s1[], const char s2[] );

void QuickSort( String *str, int first, int last );

void Swap( String *a, String *b );

int StrCount( const char buf[] );

String *CreateStringsPtrs( const char buf[], size_t *str_cnt );

void PrintStrs( const char filename[], const String strs[], size_t size );


int main( void )
{
  char FileName[MAX_NAME] = {0}, *buf = NULL;
  size_t string_size = 0;
  String *text = NULL;

  printf("# Sort strings from file program\n\n"
         "Input filename to start: ");
  int IsOk = scanf("%s", FileName);
  assert(IsOk);

  buf = LoadAndCreateStrings(FileName);
  assert(buf != NULL);

  text = CreateStringsPtrs(buf, &string_size);
  assert(text != NULL);

  QuickSort(text, 0, string_size - 1);

  printf("Input filename to save: ");
  IsOk = scanf("%s", FileName);
  assert(IsOk);

  PrintStrs(FileName, text, string_size);

  free(buf);
  free(text);
  return 0;
}

/**
 * Load strings from file function.
 * \param [in]    filename[]  String with name of the file.
 * \return Pointer to array with strings with ending '\0'.
 * \warning Dynamic memory, it is necessary to use free() function.
 */
char * LoadAndCreateStrings( const char filename[] )
{
  assert(filename != NULL);

  FILE *f = fopen(filename, "r");

  assert(f != NULL);

  if (fseek(f, 0, SEEK_END) != 0)
  {
    printf("Error in fseek() fucntion\n");
    fclose(f);
    return NULL;
  }

  int len = ftell(f);
  if (len <= 0)
  {
    printf("Incorrect file length\n");
    fclose(f);
    return NULL;
  }

  rewind(f);

  char *buf = (char *)calloc(len + 1, sizeof(buf[0]));
  fread(buf, sizeof(buf[0]), len, f);

  fclose(f);
  return buf;
} /* End of 'LoadAndCreateStrings' function */

/**
 * Create array with a pointers to strings beginings function.
 * \param [in]    buf[]     Strings.
 * \param [out]   *str_cnt  Pointer to amount of strings.
 * \return Pointer to array of String structures.
 * \warning Dynamic memory, it is necessary to use free() function.
 */
String *CreateStringsPtrs( const char buf[], size_t *str_cnt )
{
  assert(buf != NULL);
  assert(str_cnt != NULL);

  int num_of_strs = StrCount(buf), len_cnt = 0, is_new_str = 1;
  char *str = buf;
  char prev = 0;
  String *txt = (String *)calloc(num_of_strs, sizeof(txt[0]));
  *str_cnt = 0;

  prev = '\n';
  while (*str != '\0')
  {
    if (prev == '\n' && *str != '\n')
    {
      txt[*str_cnt].str = str;
      len_cnt = 0;
    }
    len_cnt++;
    if (*str == '\n' && prev != '\n')
    {
      txt[*str_cnt].len = len_cnt;
      *str_cnt++;
    }
    prev = *str;
    str++;
  }

  return txt;
} /* End of 'CreateStringsPtrs' function */

/**
 * Count strings function.
 * \param [in]    buf[]  Strings.
 * \return Number of strings.
 */
int StrCount( const char buf[] )
{
  assert(buf != NULL);

  int str_count = 0;
  char *str = buf;

  while (*str != '\0')
  {
    if (*str == '\n')
      str_count++;
    str++;
  }
  if (*(str - 1) != '\n')
    str_count++;

  return str_count;
} /* End of 'StrCount' function */

/**
 * Swap two values function by pointers.
 * \param [out]   *a  Pointer to first value.
 * \param [out]   *b  Pointer to second value.
 * \return None.
 */
void Swap( String *a, String *b )
{
  assert(a != NULL);
  assert(b != NULL);

  String tmp = *a;
  *a = *b;
  *b = tmp;
} /* End of 'Swap' function */

/**
 * Swap strings function by quick sort algorithm.
 * \param [out]    *str   Pointer to strings array.
 * \param [in]     first  Start sort index.
 * \param [in]     last   End sort index.
 * \return None.
 */
void QuickSort( String *str, int first, int last )
{
  assert(str != NULL);

  if (first >= last)
    return;

  char *mid = str[(first + last) / 2].str;
  int begin = first, end = last;

  while (begin <= end)
  {
    while (StrCompare(str[begin].str, mid) < 0)
      begin++;
    while (StrCompare(str[end].str, mid) > 0 )
      end--;
    if (begin <= end)
    {
      Swap(str + begin, str + end);
      begin++;
      end--;
    }
  };

  QuickSort(str, first, end);
  QuickSort(str, begin, last);
} /* End of 'QuickSort' function */

/**
 * Compare two strings in alphabet order function.
 * \param [in]     s1[]   First sttring.
 * \param [in]     s2[]   Second sttring.
 * \return Positive value if first is greater,
 *         0 if strings are equal,
 *         negative value if second is greater.
 */
int StrCompare( const char s1[], const char s2[] )
{
  assert(s1 != NULL);
  assert(s2 != NULL);

  char *str1 = s1, *str2 = s2;

  while (1)
  {
    while (!isalpha(*str1) && IfEndStr(*str1))
      str1++;
    while (!isalpha(*str2) && IfEndStr(*str2))
      str2++;
    if (*str1 != *str2 || !IfEndStr(*str2) || !IfEndStr(*str1))
      break;
    str1++, str2++;
    if (!IfEndStr(*str2) || !IfEndStr(*str1))
      break;
  }
  /*while (*str1 == *str2 && IfEndStr(*str1))
    str1++, str2++;*/

  return (int)* str1 - (int)* str2;
} /* End of 'StrCompare' function */

/**
 * Print strings in file function.
 * \param [in]    filename[]  String with name of the file.
 * \param [in]    strs[]      Array of strings.
 * \param [in]    size        Strings amount.
 * \return None.
 */
void PrintStrs( const char filename[], const String strs[], size_t size )
{
  assert(strs != NULL);
  assert(filename != NULL);

  FILE *f = fopen(filename, "a");

  assert(f != NULL);

  for (size_t i = 0; i < size; i++)
    for (size_t j = 0; j < strs[i].len; j++)
      fputc(strs[i].str[j], f);

  fclose(f);
} /* End of 'PrintStrs' function */