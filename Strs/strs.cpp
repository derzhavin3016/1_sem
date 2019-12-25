#include "strs.h"
#include "..\DEFS.h"

void ad6::StrCpy(  char *Dest, const  char* Source )
{
  while ((*Dest++ = *Source++) != 0);
}

ad6::string::string( void ) : str(nullptr),
                              len(0)
{
}

const  char *ad6::string::str_ptr( void ) const
{
  return str;
}


ad6::string &ad6::string::operator=( const string &s )
{
        str = s.str;
  len = s.len;
  return *this;
}

bool ad6::string::operator==( const string &s )
{
  if (StrCmp(this, &s) == 0)
    return true;
  return false;
}

bool ad6::string::operator==( string &s )
{
  if (StrCmp(this, &s) == 0)
    return true;
  return false;
}

unsigned ad6::string::size( void )
{
  return len;
}

 char ad6::string::operator[]( unsigned index ) const
{
  assert(index < len);
  return str[index];
}

ad6::string::~string( void )
{
 /* if (str != nullptr)
    delete[] str;*/
  str = nullptr;
  len = 0;
}

int ad6::StrCmp( string *str1, const string* str2 )
{
  if (str1->len > str2->len)
    return 1;
  if (str1->len < str2->len)
    return -1;

  for(unsigned i = 0; i < str1->len; i++)
    if ((*str1)[i] != (*str2)[i])
      return str1->str[i] - str2->str[i];

  return 0;
}

int ad6::StrCmp( string *str1, string* str2 )
{
  if (str1->len > str2->len)
    return 1;
  if (str1->len < str2->len)
    return -1;

  for(unsigned i = 0; i < str1->len; i++)
    if ((*str1)[i] != (*str2)[i])
      return str1->str[i] - str2->str[i];

  return 0;
}

int ad6::StriCmp( string *str1, string* str2 )
{
  return stricmp((char *)str1->str, (char *)str2->str);
}

int ad6::StrChrCmp( const  char s[], string &st )
{
  unsigned i = 0;
  for (i = 0; i < st.len && s[i] != 0; i++)
    if (s[i] != st[i])
      return s[i] - st[i];

  if (s[i] == 0 && i == st.len)
    return 0;
  if (s[i] == 0 && i < st.len)
    return -1;
  return 1;
}


int ad6::StrChrCmp( const  char s[], const string &st )
{
  unsigned i = 0;
  for (i = 0; i < st.len && s[i] != 0; i++)
    if (s[i] != st[i])
      return s[i] - st[i];

  if (s[i] == 0 && i == st.len)
    return 0;
  if (s[i] == 0 && i < st.len)
    return -1;
  return 1;
}

void ad6::string::print_in_str( char *st ) const
{
  for (size_t i = 0; i < len; i++)
    sprintf(st, "%c", str[i]);
}

ad6::string::string( const  char *s, unsigned size ) : str(s),
                                              len(size)
{
}

/**
 * \brief Print str to console fucntion.
 * \param f Pointer to file structure.
*/
std::ostream & ad6::operator<<( std::ostream &out, string &str )
{
  for (unsigned i = 0; i < str.len; i++)
    out << str.str[i];

  return out;
} /* End of 'print_in_file' function */

/**
 * \brief Print str to opened file fucntion.
 * \param f Pointer to file structure.
*/
void ad6::string::print_in_file( FILE *f ) const  
{
  assert(f != nullptr);

  for (unsigned i = 0; i < len; i++)
    fprintf(f, "%c", str[i]);
} /* End of 'print_in_file' function */
