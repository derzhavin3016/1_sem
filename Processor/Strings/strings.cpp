#include "strings.h"

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
 * \param [in]    buf[]        Strings.
 * \param [in]    buf_size     Size of buf array.
 * \param [out]   *str_cnt     Pointer to amount of strings.
 * \param [in]    end_value    Character of the end of the strings.
 * \return Pointer to array of String structures.
 * \warning Dynamic memory, it is necessary to use free() function.
 */
String *CreateStringsPtrs( const char buf[], size_t buf_size, size_t *str_cnt, char end_value )
{
  assert(buf != NULL);
  assert(buf_size != 0);
  assert(str_cnt != NULL);

  int num_of_strs = StrCount(buf, buf_size, end_value), real_str = 0;
  String *txt = (String *)calloc(num_of_strs, sizeof(txt[0]));
  assert(txt != NULL);
  
  char *s = (char *)memchr(buf, end_value, buf_size);
  const char *prev = buf;
  char prevch = 0, pre_prevch = 0;
  if (s != buf && s != NULL)
    prevch = *(s - 1);
  if (s != buf + 1 && s != NULL)
    prevch = *(s - 2);
  while (s != NULL)
  {
    txt[real_str].str = prev;
    txt[real_str++].len = s - prev + 1;
    if ((prevch == '\n' || prevch == '\r') && pre_prevch == '\n')
      real_str--;
    prev = s + 1;
    (s = (char *)memchr(prev, end_value, buf_size - (prev - 1 - buf) - 1));
    if (s != NULL)
    {
      prevch = *(s - 1);
      pre_prevch = *(s - 2);
    }
  }
  s = (char *)memchr(prev, 0, buf_size - (s - buf) - 1);
  if (*(s-1) != '\n')
  {
    txt[real_str].str = prev;
    txt[real_str++].len = s - prev + 1;
  }

  *str_cnt = real_str;
  return txt;
} /* End of 'CreateStringsPtrs' function */

/**
 * \btief Delete comments from file function.
 * \param [in, out] strs  Pointer to array with strings.
 * \param [in]      size  Size of strings array.
 * \param [in]      end_value    Character of the end of the strings.
 * \return None.
 */
void DelCom( String *strs, size_t size, char end_value )
{
  assert(strs != nullptr);
  assert(size != 0);

  for (size_t i = 0; i < size; i++)
  {
    char *s = (char *)memchr(strs[i].str, ';', strs[i].len);
    if (s != nullptr)
    {
      *s = end_value;
      strs[i].len = s - strs[i].str + 1;
    }
  }
} /* End of 'DelCom' function */

/**
 * Count strings function.
 * \param [in]    buf[]     Strings.
 * \param [in]    buf_size  Size of buf array.
 * \param [in]    end_value Character of the end of the strings.
 * \return Number of strings.
 */
int StrCount( const char buf[], size_t buf_size, char end_value )
{
  assert(buf != NULL);
  assert(buf_size != 0);

  int str_count = 0;
  char *s = (char *)memchr(buf, end_value, buf_size); 
  while (s != NULL)
  {
    str_count++;
    s = (char *)memchr(s + 1, end_value, buf_size - (s - buf) - 1);
  }
  if (buf[buf_size - 1] != end_value)
    str_count++;

  return str_count;
} /* End of 'StrCount' function */


/**
 * Compare two strings in alphabet order function from the beginings.
 * \param [in]     s1[]   First string.
 * \param [in]     s2[]   Second string.
 * \return Positive value if first is greater,
 *         0 if strings are equal,
 *         negative value if second is greater.
 */
int StrCompareBegin( const char s1[], const char s2[] )
{
  assert(s1 != NULL);
  assert(s2 != NULL);

  const unsigned char *str1 = (unsigned char *)s1, 
                      *str2 = (unsigned char *)s2;

  while (1)
  {
    while (!isalpha(*str1) && IfEndStr(*str1))
      str1++;
    while (!isalpha(*str2) && IfEndStr(*str2))
      str2++;
    if (toupper(*str1) != toupper(*str2) || !IfEndStr(*str2) || !IfEndStr(*str1))
      break;
    str1++, str2++;
    if (!IfEndStr(*str2) || !IfEndStr(*str1))
      break;
  }
  /*while (*str1 == *str2 && IfEndStr(*str1))
    str1++, str2++;*/

  return toupper(*str1) - toupper(*str2);
} /* End of 'StrCompareBegin' function */

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

  FILE *f = fopen(filename, "wb");


  assert(f != NULL);

  for (size_t i = 0; i < size; i++)
    fwrite(strs[i].str, sizeof(strs[0].str[0]), strs[i].len, f);

  fclose(f);
} /* End of 'PrintStrs' function */

/**
 * \brief Strings concatenation function.
 * \param   Dest     string to write to the back.
 * \param   Src      string to copy from.
 * \return None.
 */
void StrConcat( char Dest[], const char Src[] )
{
  unsigned char *dst = (unsigned char *)Dest;
  unsigned char *src = (unsigned char *)Src;

  while (*dst != 0)
    dst++;

  while ((*dst = *src) != 0)
    dst++, src++;
} /* End of 'StrConcat' function */

/**
 * \brief Put buffer to file function.
 * \param file_name  Name of a file to write.
 * \param buf        Buffer to put.
 * \param buf_size   Size of a buffer.
 * \return true if all is ok.
 * \return false otherwise.
 */
bool PutBufToFile( const char file_name[], const char buf[], size_t buf_size )
{
  assert(file_name != nullptr);
  assert(buf != nullptr);
  assert(buf_size != 0);

  FILE *out = fopen(file_name, "wb");

  if (out == nullptr)
    return false;


  fwrite(buf, sizeof(char), buf_size, out);

  fclose(out);
  return true;
} /* End of 'PutBufToFile' function */