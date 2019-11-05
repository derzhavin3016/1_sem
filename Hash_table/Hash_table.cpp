#include "Hash_table.h"



/**
 * \brief Clear hash table function
 */
void ad6::hash_table::Clear( void )
{
  for (int i = 0; i < TABLE_SIZE; i++)
    table[i].Kill();
} /* End of 'Clear' function */

/**
 * \brief LoadTxt from file function
 * \param [in] filename  Name of a file to read from.
 * \return true if all is OK
 * \return false otherwise
 */
bool ad6::hash_table::LoadTxt( const char filename[] )
{
  assert(filename != nullptr);

  buf = FillBuf(filename, &buf_size);
  if (buf == nullptr)
    return false;

  strs = CreateStringsPtrs(buf, buf_size, &strs_size, '\n');
  if (strs == nullptr)
  {
    printf("Error in creating strings.\n");
    return false;
  }

  return true;
} /* End of 'LoadTxt' function */

/**
 * \brief Hash all words from buf function.
 * \param [in] Hash pointer to hash function.
 */
void ad6::hash_table::Hashing( hash_t (*Hash)( const String *str ) )
{
  for (size_t cnt = 0; cnt < strs_size; cnt++)
  {
    hash_t h = Hash(strs + cnt);
    
    table[h % TABLE_SIZE].Push_tail(strs[cnt].str);
  }
} /* End of 'Hashing' function */


/**
 * \brief Put data to CSV file function
 * \param [in] file_out  Name of a file to read from.
 * \param [in] sep_sym   Separation symbol
 * \param [in] name      Name of hash function
 * \return true if all is OK
 * \return false otherwise
 */
bool ad6::hash_table::PutToCSV( const char file_out[], char sep_sym, const char name[] )
{
  FILE *F = fopen(file_out, "a");
  if (F == nullptr)
  {
    printf("Error with file opening");
    return false;
  }

  if (!IsHeaderPrint)
    PrintCSVHeader(F, sep_sym);

  fprintf(F, "%s", name);
  for (size_t i = 0; i < TABLE_SIZE; i++)
  {
    fprintf(F, "%4d%c", table[i].GetSize(), sep_sym);
  }
  fprintf(F, "\n");

  fclose(F);
  return true;
} /* End of 'PutToCSV' function */

/**
 * \brief Print Header to .csv file function
 * \param [in] f  Pointer to opened file to read from.
 * \param [in] sep_sym   Separation symbol
 */
void ad6::hash_table::PrintCSVHeader( FILE* f,  char sep_sym )
{
  assert(f != nullptr);

  fprintf(f, "List; ");
  for (size_t i = 0; i < TABLE_SIZE; i++)
  {
    fprintf(f, "%4d%c", i, sep_sym);
  }
  fprintf(f, "\n");

  IsHeaderPrint = true;
} /* End of 'PrintCSVHeader' function */
