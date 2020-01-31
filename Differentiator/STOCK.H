/* FILE NAME: stock.h
 * PROGRAMMER: AD6
 * LAST UPDATE: 14.07.2018
 * PURPOSE: Stock handle;
 */

#ifndef __STOCK_H_
#define __STOCK_H_

#include <cassert>

 namespace ad6
 {
   template <typename data>
   int stk_def_cmp( data &a, const data &b )
   {
     return !(a == b);
   }

   template <typename data>
   class stock
   {
   private:
     size_t real_size, max_size;
     data *array;

   public:

     // Default constructor
     stock( void ) : array(nullptr),                     real_size(0),                      max_size(0)     {     }

     stock( size_t size ) : array(new data[size]),
                            real_size(0),
                            max_size(size)
     {
     
     }

     // Distructor
     ~stock( void )     {       if (array != nullptr)         delete[] array;       array = nullptr;       real_size = 0;     }


     // Copy constructor by constant link
     stock( const stock<data> &CopyData ) : array(new data[CopyData.max_size]),                                            real_size(CopyData.real_size),                                            max_size(CopyData.max_size)     {       for (size_t i = 0; i < real_size; i++)         array[i] = CopyData.array[i];     }


     // Assignment operator
     stock<data> & operator=( const stock<data> & CopyData )     {       if (array != nullptr)         delete[] array;           array = new data[CopyData.max_size];       real_size = CopyData.real_size;       max_size = CopyData.max_size;       for (size_t i = 0; i < real_size; i++)         array[i] = CopyData.array[i];       return *this;     }


     /**
      * \brief Add new element to stock function.
      * \param NewData  link to new element to add.
      * \return link to stock class.
      */
     stock<data> & add( const data &NewData )
     {
       if (real_size >= max_size)
       {
         if (max_size == 0)
           max_size = 1;
         else
           max_size *= 2;
        resize(max_size);
       }
       if (array == nullptr)
         resize(max_size);
       array[real_size++] = NewData;
       return *this;
     }

     void resize( size_t size )
     {
        data *new_bulk(new data[size]);

        if (array != nullptr)
        {
          for (size_t i = 0; i < real_size; i++)            new_bulk[i] = array[i];

          delete[] array;
        }
        array = new_bulk;
        max_size = size;
     }

     size_t size( void )
     {
       return real_size;
     }


     void clear( void )
     {
       real_size = 0;
     }


     stock<data> & operator<<( const data &NewData )
     {
       return add(NewData);
     }



     data & operator[]( const int Index )
     {
       assert(Index >= 0 && Index <= (int)max_size);
       return array[Index];
     }


     data & operator[]( const int Index ) const
     {
       assert(Index >= 0 && Index <= (int)max_size);
       return array[Index];
     }     template <class T2>       void walk( const T2 &Walker )       {         for (int i = 0; i < real_size; i++)           Walker(array[i]);        }

      /**
       * \brief Find value in stock function
       * \param Data link to data to find.
       * \return number of element in array if it was find
       * \return -1 otherwise
       */
      int find( const data &Data, int (*comp)( data &a, const data &b ) = nullptr )
      {
        if (comp == nullptr)
          comp = stk_def_cmp;

        for (size_t i = 0; i < real_size; i++)
          if (comp(array[i], Data) == 0)
            return (int)i;

        return -1;
      } /* End of 'find' function */
   };
}
#endif /* __STOCK_H_ */


/* End of 'stock.h' file */