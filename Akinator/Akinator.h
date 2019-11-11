#ifndef __AKINATOR_H_
#define __AKINATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <assert.h>
//#include "D:\TX\TXLib.h"

#define DEF_NAME "Pig"

#pragma warning (disable: 4996)

namespace ad6
{
  const size_t ANSWER_MAX = 100;
  const size_t QUEST_MAX = 200;
  struct Node
  {
    char quest[QUEST_MAX];
    Node *right;
    Node *left;
    
    // default constructor
    Node( void ) : quest{},
                   right(nullptr),
                   left(nullptr)
    {
    }


    // Constructor by string
    Node( const char str[] ) : quest{},
                         right(nullptr),
                         left(nullptr)
    {
      strcpy(quest, str);
    }

    ~Node( void )
    {
      memset(quest, 0, QUEST_MAX);
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

  public:
    // Default constructor
    Aki( void ) : root(new Node(DEF_NAME))
    {
    }

    bool SaveTree( const char filename[] );

    bool ReadTree( const char filename[] );

    void Play( void );

    void ProcessLoop( void );

    void Dump( void );

    // Destructor
    ~Aki( void )
    {
      if (root != nullptr)
        delete root;
    }
  private:

    bool PrintTree( FILE *f, Node *node );

    bool BuildTree( FILE *f, Node *node );

    Node * Guess( Node *node, int *IsOk );

    bool CreateNodes( Node *prnt );
  };
}


#endif /* __AKINATOR_H_ */

