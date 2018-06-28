#include "utils/math/bcintIterators/iterator.h"

namespace Util
{
namespace Math
{
BCInt::iterator::iterator (bool half, list< unsigned char >::iterator digit) :
    lowerHalf (half), currByte (digit)
{}

bool BCInt::iterator::operator== (const iterator& other) const
{
    return lowerHalf == other.lowerHalf && currByte == other.currByte;
}

bool BCInt::iterator::operator!= (const iterator& other) const
{
    return !(*this == other);
}

unsigned char BCInt::iterator::operator* () const
{
    unsigned char byte = *currByte;
    if (lowerHalf)
    {
        byte &= 15;
    }
    else
    {
        byte >>= 4;
    }

    return byte;
}

BCInt::iterator& BCInt::iterator::operator++ () //pre
{
    if (lowerHalf)
    {
        lowerHalf = false;
    }
    else
    {
        lowerHalf = true;
        currByte++;
    }

    return *this;
}

BCInt::iterator BCInt::iterator::operator++ (int) //post
{
    iterator retval = *this;
    ++*this;
    return retval;
}

BCInt::iterator& BCInt::iterator::operator-- ()
{
    if (!lowerHalf)
    {
        lowerHalf = true;
    }
    else
    {
        lowerHalf = false;
        currByte--;
    }

    return *this;
}

BCInt::iterator BCInt::iterator::operator-- (int)
{
    iterator retval = *this;
    --*this;
    return retval;
}

unsigned char BCInt::iterator::get () const
{
    return **this;
}

void BCInt::iterator::set (unsigned char nybble)
{
    unsigned char& byte = *currByte;

    if (nybble >= 10)
    {
        return;
    }

    if (lowerHalf)
    {
        byte &= 0b11110000;
        byte |= nybble;
    }
    else
    {
        byte &= 0b00001111;
        byte |= nybble << 4;
    }
}
} // namespace Math
} // namespace Util