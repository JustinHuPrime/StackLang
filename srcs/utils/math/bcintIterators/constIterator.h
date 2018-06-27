#ifndef BCINTCITERATOR_H
#define BCINTCITERATOR_H

#include "utils/math/bcint.h"

#include <list>
using std::list;

class BCInt::const_iterator
{
  public:
    const_iterator (bool, list< unsigned char >::const_iterator);

    bool operator== (const const_iterator&) const;
    bool operator!= (const const_iterator&) const;

    unsigned char operator* () const;

    const_iterator& operator++ ();   //pre
    const_iterator operator++ (int); //post
    const_iterator& operator-- ();
    const_iterator operator-- (int);

    unsigned char get () const;

  private:
    bool lowerHalf;
    list< unsigned char >::const_iterator currByte;
};

#endif /* BCINTCITERATOR_H */