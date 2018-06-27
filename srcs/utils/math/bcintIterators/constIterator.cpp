#include "utils/math/bcintIterators/constIterator.h"

BCInt::const_iterator::const_iterator (bool half, list< unsigned char >::const_iterator digit) :
    lowerHalf (half), currByte (digit)
{}

bool BCInt::const_iterator::operator== (const const_iterator& other) const
{
    return lowerHalf == other.lowerHalf && currByte == other.currByte;
}

bool BCInt::const_iterator::operator!= (const const_iterator& other) const
{
    return !(*this == other);
}

unsigned char BCInt::const_iterator::operator* () const
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

BCInt::const_iterator& BCInt::const_iterator::operator++ () //pre
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

BCInt::const_iterator BCInt::const_iterator::operator++ (int) //post
{
    const_iterator retval = *this;
    ++*this;
    return retval;
}

BCInt::const_iterator& BCInt::const_iterator::operator-- ()
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

BCInt::const_iterator BCInt::const_iterator::operator-- (int)
{
    const_iterator retval = *this;
    --*this;
    return retval;
}

unsigned char BCInt::const_iterator::get () const
{
    return **this;
}