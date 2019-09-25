#include "onegin.h"

const int MAX_NAME = 1000;

int main( void )
{
  char file_name[MAX_NAME] = {{0}};
  size_t string_size = 0, file_length = 0;
  int err_code;

  int IsOk = Input("# Sort strings from the file program.\n\n"
                   "Input file name to start: ", "%s", file_name);
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

  IsOk = Input("Input filename to save: ", "%s", file_name);
  assert(IsOk);
  
  int sep_width = 0, sep_height = 0;
  char sep_sym = 0;

  IsOk = Input("Input the height of separation: ", "%d", &sep_height);
  assert(IsOk);
  IsOk = Input("Input the width of separation: ", "%d", &sep_width);
  assert(IsOk);
  IsOk = Input("Input the separation symbol: ", "\n%c", &sep_sym);
  assert(IsOk);
  
  PrintStrs(file_name, text, string_size);

  QuickSort(text, 0, string_size - 1, StrCompareEnd);
  StrSeparate(file_name, sep_height, sep_width, sep_sym);
  PrintStrs(file_name, text, string_size);
  StrSeparate(file_name, sep_height, sep_width, sep_sym);
  PrintStr(file_name, buf);

  free(buf);
  free(text);
  return 0;
}

