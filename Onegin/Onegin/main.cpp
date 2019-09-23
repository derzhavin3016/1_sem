#include "onegin.h"

const int MAX_NAME = 1000;

int main( void )
{
  char file_name[MAX_NAME] = {{0}};
  size_t string_size = 0, file_length = 0;
  int err_code;

  printf("# Sort strings from file program\n\n"
         "Input filename to start: ");
  int IsOk = scanf("%s", file_name);
  assert(IsOk);

  char *buf = LoadAndCreateStrings(file_name, &file_length, &err_code);
  if (buf == NULL)
  {
    LACS_Process_Error(err_code);
    return 1;
  }

  String *text = CreateStringsPtrs(buf, file_length, &string_size, '\n');
  assert(text != NULL);

  QuickSort(text, 0, string_size - 1, StrCompareBegin);

  printf("Input filename to save: ");
  IsOk = scanf("%s", file_name);
  assert(IsOk);
  
  int sep_wide;
  printf("Input the width of separation: ");
  IsOk = scanf("%d", &sep_wide);
  assert(IsOk);


  PrintStrs(file_name, text, string_size);

  QuickSort(text, 0, string_size - 1, StrCompareEnd);
  StrSeparate(file_name, sep_wide);
  PrintStrs(file_name, text, string_size);
  StrSeparate(file_name, sep_wide);
  PrintStr(file_name, buf);

  free(buf);
  free(text);
  return 0;
}

