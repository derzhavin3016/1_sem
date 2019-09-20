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

  String *text = CreateStringsPtrs(buf, &string_size, '\n');
  assert(text != NULL);

  QuickSort(text, 0, string_size - 1, StrCompareBegin);

  printf("Input filename to save: ");
  IsOk = scanf("%s", file_name);
  assert(IsOk);

  PrintStrs(file_name, text, string_size);

  QuickSort(text, 0, string_size - 1, StrCompareEnd);
  Separate(file_name, 100);
  PrintStrs(file_name, text, string_size);
  Separate(file_name, 100);
  PrintStr(file_name, buf);

  free(buf);
  free(text);
  return 0;
}

