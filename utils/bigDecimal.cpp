#include "utils/bigDecimal.h"
#include <cmath>
#include <stdexcept>
using std::invalid_argument;
using std::to_string;

BigDecimal::BigDecimal (const string& s)
{
    if (s.find ('.') != string::npos) //has a decimal point
    {
        string whole = s.substr (0, s.find ('.'));
        string decimal = s.substr (s.find ('.') + 1);
    }
    else //doesn't have decimal point
    {
        string whole = s;
    }
}


BigDecimal::operator string () const
{
    // TODO
}


const BigDecimal BigDecimal::operator+ () const
{
    return *this;
}

const BigDecimal BigDecimal::operator- () const
{
    BigDecimal retVal = *this;
    retVal.negative = !retVal.negative;
    return retVal;
}


BigDecimal& BigDecimal::operator++ () //pre
{
    // TODO

    return *this;
}

BigDecimal BigDecimal::operator++ (int) //post
{
    BigDecimal retVal = *this;
    ++(*this);
    return retVal;
}

BigDecimal& BigDecimal::operator-- ()
{
    // TODO

    return *this;
}

BigDecimal BigDecimal::operator-- (int)
{
    BigDecimal retVal = *this;
    --(*this);
    return retVal;
}


const BigDecimal BigDecimal::operator+ (const BigDecimal& other) const
{
    return BigDecimal (*this) += other;
}

const BigDecimal BigDecimal::operator- (const BigDecimal& other) const
{
    return BigDecimal (*this) -= other;
}

const BigDecimal BigDecimal::operator* (const BigDecimal& other) const
{
    return BigDecimal (*this) *= other;
}

const BigDecimal BigDecimal::operator/ (const BigDecimal& other) const
{
    return BigDecimal (*this) /= other;
}

const BigDecimal BigDecimal::operator% (const BigDecimal& other) const
{
    return BigDecimal (*this) %= other;
}


BigDecimal& BigDecimal::operator+= (const BigDecimal& other)
{
    // TODO
}

BigDecimal& BigDecimal::operator-= (const BigDecimal& other)
{
    // TODO
}

BigDecimal& BigDecimal::operator*= (const BigDecimal& other)
{
    // TODO
}

BigDecimal& BigDecimal::operator/= (const BigDecimal& other)
{
    // TODO
}

BigDecimal& BigDecimal::operator%= (const BigDecimal& other)
{
    // TODO
}


bool BigDecimal::operator== (const BigDecimal& other) const
{
    if (negative != other.negative)
    {
        return false;
    }

    auto meWholeIt = wholePart.begin ();
    auto otherWholeIt = other.wholePart.begin ();
    bool leadingZeroes = true;
    
    while (meWholeIt != wholePart.end () && otherWholeIt != other.wholePart.end ())
    {
        if (*meWholeIt == 0 || *otherWholeIt == 0) // discard up to 1 leading zero
        {
            leadingZeroes = leadingZeroes && *meWholeIt == 0 && *otherWholeIt == 0;
            
            if (*meWholeIt == 0)
            {
                meWholeIt++;
            }
            if (*otherWholeIt == 0)
            {
                otherWholeIt++;
            }

            continue;
        }

        if (*meWholeIt)
    }
}

bool BigDecimal::operator!= (const BigDecimal& other) const
{
    return !(*this == other);
}

bool BigDecimal::operator< (const BigDecimal& other) const
{}

bool BigDecimal::operator<= (const BigDecimal& other) const
{}

bool BigDecimal::operator> (const BigDecimal& other) const
{}

bool BigDecimal::operator>= (const BigDecimal& other) const
{}


ostream& operator<< (ostream&, const BigDecimal&)
{}

istream& operator>> (istream&, const BigDecimal&)
{}

