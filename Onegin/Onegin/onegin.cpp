#include "onegin.h"

/**
 * Load strings from file function.
 * \param [in]       filename[]  String with name of the file.
 * \param [in, out]  *str_count  Pointer to value with strings amount.
 * \param [in, out]  *error_code  Pointer to error code.
 * \return Pointer to array with strings with ending '\0', if error is detected, return NULL.
 * \warning Dynamic memory, it is necessary to use free() function.
 * \note Write to the error_code 
 *       LACS_FSEEK_ERROR if error detected in fseek() fucntion,
 *       LACS_LENGTH_ERROR if error detected in length counting,
 *       LACS_BUF_MEMORY_ERROR if error in memory allocation for buffer,
 *       LACS_FOPEN_ERROR if error is detected upon attempt to open file.
 */
char * LoadAndCreateStrings( const char filename[], size_t *str_count, int *error_code )
{
  assert(filename != NULL);
  assert(error_code != NULL);

  FILE *f = fopen(filename, "rb");

  if (f == NULL)
  {
    *error_code = LACS_FOPEN_ERROR;
    return NULL;
  }

  int len = FileLength(f);
  if (len == LACS_LENGTH_ERROR || len == LACS_FSEEK_ERROR)
  {
    fclose(f);
    *error_code = len;
    return NULL;
  }

  char *buf = (char *)calloc(len + 1, sizeof(buf[0]));
  if (buf == NULL)
  {
    fclose(f);
    *error_code = LACS_BUF_MEMORY_ERROR;
    return NULL;
  }
  len = fread(buf, sizeof(buf[0]), len, f);

  *str_count = len;
  fclose(f);
  return buf;
} /* End of 'LoadAndCreateStrings' function */

/**
 * Measure open file length function.
 * \param [in]       *f  Pointer to file.
 * \return Length of a file if all is OK,
 *         LACS_FSEEK_ERROR if error detected in fseek() fucntion,
 *         LACS_LENGTH_ERROR if error detected in length counting.
 */
int FileLength( FILE *f )
{
  assert(f != NULL);

  if (fseek(f, 0, SEEK_END) != 0) 
    return LACS_FSEEK_ERROR;

  int len = ftell(f);
  if (len <= 0)
    return LACS_LENGTH_ERROR;

  rewind(f);
  return len;
} /* End of 'FileLength' function */

/**
 * Processing errors in LACS fucntion.
 * \param [in]  error_code  Error code value.
 * \return None.
 */
void LACS_Process_Error( int error_code )
{
  switch (error_code)
  {
  case LACS_LENGTH_ERROR:
    printf("ERROR. Incorrect file length.\n");
    break;
  case LACS_FOPEN_ERROR:
    printf("ERROR. Error with file openning.\n");
    break;
  case LACS_FSEEK_ERROR:
    printf("ERROR. Error with fseek() function.\n");
    break;
  case LACS_BUF_MEMORY_ERROR:
    printf("ERROR. Error with allocation memory for the buffer.\n");
    break;
  default:
    printf("Incorrect error code.\n");
    break;
  }
} /* End of 'LACS_Process_Error' function */

/**
 * Create array with a pointers to strings beginings function.
 * \param [in]    buf[]     Strings.
 * \param [out]   *str_cnt  Pointer to amount of strings.
 * \param [in]    end_value Character of the end of the strings.
 * \return Pointer to array of String structures.
 * \warning Dynamic memory, it is necessary to use free() function.
 */
String *CreateStringsPtrs( const char buf[], size_t *str_cnt, char end_value )
{
  assert(buf != NULL);
  assert(str_cnt != NULL);

  int num_of_strs = StrCount(buf, end_value), len_cnt = 0,
    is_new_str = 1, str_amount = 0;
  const char *str = buf;
  char prev = 0;
  String *txt = (String *)calloc(num_of_strs, sizeof(txt[0]));
  assert(txt != NULL);

  prev = '\n';
  while (*str != '\0')
  {
    if (*str == '\r')
    {
      len_cnt++;
      str++;
      continue;
    }
    if (prev == '\n' && *str != '\n')
    {
      txt[str_amount].str = str;
      len_cnt = 0;
    }
    len_cnt++;
    if (*str == '\n' && prev != '\n')
    {
      txt[str_amount].len = len_cnt;
      str_amount++;
    }
    prev = *str;
    str++;
  }

  *str_cnt = str_amount;
  return txt;
} /* End of 'CreateStringsPtrs' function */

/**
 * Count strings function.
 * \param [in]    buf[]     Strings.
 * \param [in]    end_value Character of the end of the strings.
 * \return Number of strings.
 */
int StrCount( const char buf[], char end_value )
{
  assert(buf != NULL);

  int str_count = 0;
  const char *str = buf;

  while (*str != '\0')
  {
    if (*str == end_value)
      str_count++;
    str++;
  }
  if (*(str - 1) != end_value)
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
 * Sort strings function by quick sort algorithm.
 * \param [out]    *str   Pointer to strings array.
 * \param [in]     first  Start sort index.
 * \param [in]     last   End sort index.
 * \param [in]     *Comp  Pointer to compare function.
 * \return None.
 * \note  compare fucntion need to return positive value, 
 *        if first string is 'greater', 
 *        zero if strings are equal,
 *        negative value, if second string is greater.
 */
void QuickSort( String *str, int first, int last, int (*Comp)( const String *s1, const String *s2 ) )
{
  assert(str != NULL);

  if (first >= last)
    return;

  const String *mid = &str[(first + last) / 2];
  int begin = first, end = last;

  while (begin <= end)
  {
    while (Comp(str + begin, mid) < 0)
      begin++;
    while (Comp(str + end, mid) > 0 )
      end--;
    if (begin <= end)
    {
      Swap(str + begin, str + end);
      begin++;
      end--;
    }
  };

  QuickSort(str, first, end, Comp);
  QuickSort(str, begin, last, Comp);
} /* End of 'QuickSort' function */

/**
 * Compare two strings in alphabet order function from the beginings.
 * \param [in]     s1[]   First string structure.
 * \param [in]     s2[]   Second string structure.
 * \return Positive value if first is greater,
 *         0 if strings are equal,
 *         negative value if second is greater.
 */
int StrCompareBegin( const String *s1, const String *s2 )
{
  assert(s1 != NULL);
  assert(s2 != NULL);
  assert(s1->str != NULL);
  assert(s2->str != NULL);

  const char *str1 = s1->str, *str2 = s2->str;

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
} /* End of 'StrCompareBegin' function */

/**
 * Find minimum between two values.
 * \param [in]     a   First value.
 * \param [in]     b   Second value.
 * \return Minimum value.
 */
__inline size_t Min( size_t a, size_t b )
{
  return a < b ? a : b;
} /* End of 'Min' function */

/**
 * Compare two strings in alphabet order function from the endings.
 * \param [in]     s1[]   First string structure.
 * \param [in]     s2[]   Second string structure.
 * \return Positive value if first is greater,
 *         0 if strings are equal,
 *         negative value if second is greater.
 */
int StrCompareEnd( const String *s1, const String *s2 )
{
  assert(s1 != NULL);
  assert(s2 != NULL);
  assert(s1->str != NULL);
  assert(s2->str != NULL);

  const char *str1 = s1->str + s1->len - 1, *str2 = s2->str + s2->len - 1;

  while (1)
  {
    while (!isalpha(*str1) && *str1 != *s1->str && IfEndStr(*str1))
      str1--;
    while (!isalpha(*str2) && *str2 != *s2->str && IfEndStr(*str2))
      str2--;
    if (*str1 != *str2 || *str1 == *s1->str || *str2 == *s2->str)
      break;
    str1--, str2--;
    if (*str1 == *s1->str || *str2 == *s2->str)
      break;
  }
  /*while (*str1 == *str2 && IfEndStr(*str1))
    str1++, str2++;*/

  return (int)* str1 - (int)* str2;
} /* End of 'StrCompareEnd' function */

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

  FILE *f = fopen(filename, "ab");

  assert(f != NULL);

  for (size_t i = 0; i < size; i++)
    fwrite(strs[i].str, sizeof(strs[0].str[0]), strs[i].len, f);

  fclose(f);
} /* End of 'PrintStrs' function */

/**
 * Print one string in file function (with '\0' ending).
 * \param [in]    filename[]  String with name of the file.
 * \param [in]    str[]      String.
 * \return None.
 */
void PrintStr( const char filename[], const char str[] )
{
  assert(str != NULL);
  assert(filename != NULL);

  FILE *f = fopen(filename, "ab");

  assert(f != NULL);

  fputs(str, f);
  
  fclose(f);
} /* End of 'PrintStrs' function */

/**
 * Separate strings in file fucntion.
 * \param [in]  file_name   Name of a file to separate.
 * \param [in]  size        Height of separation strings.
 * \return None.
 */
void Separate( const char file_name[], int size )
{
  for (int i = 0; i < size; i++)
    PrintStr(file_name, "###############################################################\n");
} /* End of 'Separate' function */