#ifndef BCINTITERATOR_H
#define BCINTITERATOR_H

#include "utils/math/bcint.h"

#include <list>

namespace Util
{
namespace Math
{
using std::list;

class BCInt::iterator
{
  public:
    iterator (bool, list< unsigned char >::iterator);

    bool operator== (const iterator&) const;
    bool operator!= (const iterator&) const;

    unsigned char operator* () const;

    iterator& operator++ ();   //pre
    iterator operator++ (int); //post
    iterator& operator-- ();
    iterator operator-- (int);

    unsigned char get () const;
    void set (unsigned char);

  private:
    bool lowerHalf;
    list< unsigned char >::iterator currByte;
};
} // namespace Math
} // namespace Util

#endif /* BCINTITERATOR_H */