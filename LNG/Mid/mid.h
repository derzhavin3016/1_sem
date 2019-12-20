#ifndef __MID_H_
#define __MID_H_


#include "..\Tree\tree.h"

namespace ad6
{
  class mid : public base_translator
  {
  private:
    node *diff;
    int symp_counter;
    bool is_diff_init;
  public:
    // Default constructor
    mid( void ) :  diff(nullptr),
                   symp_counter(0),
                   is_diff_init(false)
    {
    }

    virtual void translate( const char file_in[], const char file_out[] ) override;

    // Destructor
    ~mid( void )
    {
      if (diff != nullptr)
        delete diff;
    }
    node *get_root( void )
    {
      return root;
    }

  private:


    bool tree_diff( node **start );

    bool _calc_tree( node *nd );

    bool _simplifier( node **nd );

    bool _find_var_tree( node *start, size_t var_num ) const;

    node* _rec_symp( node *nd );

    void _smpl_tree( node **nd );

    bool _is_calc( node *nd ) const;

    node & rec_diff( node &nd, size_t var_num );

    double rec_calc( node *nd ) const;
  };
}

#endif // !__MID_H_

