#ifndef __AKINATOR_H_
#define __AKINATOR_H_

#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <assert.h>
#include "D:\TX\TXLib.h"
#include "..\List\List.h"
#include "..\Stack\stack.h"

#define DEF_NAME "Pig"

#pragma warning (disable: 4996)

namespace ad6
{
  #include <stdexcept>

  struct Error : public std::runtime_error
  {
    int code;
    const char* descr;
    int line;
    const char* file;
    const char* func;
    Error* reason;

    virtual const std::string what() 
    {
      return descr; 
    }
  };

  const size_t ANSWER_MAX = 100;
  const size_t QUEST_MAX = 200;
  struct Node
  {
    char *quest;
    Node *parent;
    Node *right;
    Node *left;
    
    // default constructor
    Node( void ) : quest(new char[ANSWER_MAX]),
                   right(nullptr),
                   left(nullptr),
                   parent(nullptr)
    {
    }


    // Constructor by string
    Node( const char str[] ) : quest(new char[ANSWER_MAX]),
                               right(nullptr),
                               left(nullptr),
                               parent(nullptr)
    {
      strcpy(quest, str);
    }

    ~Node( void )
    {
      if (right != nullptr)
      {
        delete right;
        right = nullptr;
      }
      if (left != nullptr)
      {
        delete left;
        left = nullptr;
      }
      if (quest != nullptr)
        delete[] quest;
      parent = nullptr;
    }
  };

  /**
   * \brief Swap two values function by pointers (template).
   * \param [out]   *a  Pointer to first value.
   * \param [out]   *b  Pointer to second value.
   * \return None.
   */
  template <typename Data>
  void Swap( Data* A, Data *B )
  {
    Data tmp = *A;
    *A = *B;
    *B = tmp;
  } /* End of 'Swap' function */


  class Aki
  {
  private:
    Node *root;
    List<char *> Base;

  public:
    // Default constructor
    Aki( void ) : root(new Node(DEF_NAME)),
                  Base()
    {
      Base.Push_head(root->quest);
    }

    bool SaveTree( const char filename[] ) const;

    bool ReadTree( const char filename[] );

    void Play( void );

    void ProcessLoop( void );

    void Character( const char name[] ) const;

    bool Dump( const char filename[] ) const;

    // Destructor
    ~Aki( void )
    {
      if (root != nullptr)
        delete root;

    }
  private:

    
    void RecDump( FILE *dmp, Node *node ) const;

    bool PrintTree( FILE *f, Node *node ) const;

    bool BuildTree( FILE *f, Node *node );

    Node * Guess( Node *node, int *IsOk );

    bool CreateNodes( Node *prnt );

    Node *Find( const char name[], Node *node ) const;
  };
}


#endif /* __AKINATOR_H_ */

