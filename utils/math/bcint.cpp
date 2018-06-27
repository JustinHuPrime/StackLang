#include "utils/math/bcint.h"

#include "utils/math/bcintIterators/constIterator.h"
#include "utils/math/bcintIterators/iterator.h"

#include <cmath>
#include <stdexcept>
using std::invalid_argument;
using std::log10;

BCInt::BCInt (long number) :
    digits (), negative (number < 0)
{
    if (number == 0)
    {
        digits.push_front (0b1111);
        return;
    }

    unsigned char leftDigit, rightDigit, digit;

    number = abs (number);

    while ((floor (log10 (number)) + 1) >= 2)
    {
        rightDigit = number % 10;
        number /= 10;
        leftDigit = number % 10;
        number /= 10;

        digit = rightDigit + (leftDigit << 4);

        digits.push_front (digit);
    }

    if (number != 0)
    {
        digit = number;
        digit |= 0b11110000;
        digits.push_front (digit);
    }
    else
    {
        digit = 0b1111;
        digits.push_front (digit);
    }
}

BCInt::BCInt (const string& s) :
    digits ()
{
    if (s.size () == 0 || (s[0] != '-' && s[0] != '+' && !isdigit (s[0])) ||
        (!isdigit (s[0]) && s.size () <= 1))
    {
        throw invalid_argument (s);
    }

    negative = s[0] == '-';

    unsigned currDigit = s.size () - 1;
    unsigned leftMostDigit = isdigit (s[0]) ? 0 : 1;

    if (s[leftMostDigit] == '0')
    {
        digits.push_front (0b1111);
    }

    unsigned char leftDigit, rightDigit, digit;

    while (currDigit - leftMostDigit >= 2)
    {
        rightDigit = s[currDigit] - '0';
        currDigit--;
        leftDigit = s[currDigit] - '0';
        currDigit--;

        digit = rightDigit + (leftDigit << 4);

        digits.push_front (digit);
    }

    if (currDigit != leftMostDigit)
    {
        digit = s[currDigit] - '0';
        digit |= 0b11110000;
        digits.push_front (digit);
    }
    else
    {
        digit = 0b1111;
        digits.push_front (digit);
    }
}

string BCInt::to_string () const
{
    string buffer;

    if (negative)
    {
        buffer += "-";
    }

    if (digits.size () == 0)
    {
        buffer += '0';
        return buffer;
    }

    auto iter = cbegin ();

    while (iter != cend ())
    {
        buffer += char(*iter + '0');
        iter++;
    }

    return buffer;
}

const BCInt BCInt::operator+ () const
{
    return *this;
}

const BCInt BCInt::operator- () const
{
    BCInt retval = *this;
    retval.negative = !retval.negative;
    return retval;
}

BCInt& BCInt::operator++ () // pre
{
    *this += 1;
    return *this;
}

BCInt BCInt::operator++ (int) // post
{
    BCInt retval = *this;
    ++*this;
    return retval;
}

BCInt& BCInt::operator-- ()
{
    *this -= 1;
    return *this;
}

BCInt BCInt::operator-- (int)
{
    BCInt retval = *this;
    --*this;
    return retval;
}

const BCInt BCInt::operator+ (const BCInt& other) const
{
    BCInt retval = *this;
    retval += other;
    return retval;
}

const BCInt BCInt::operator- (const BCInt& other) const
{
    BCInt retval = *this;
    retval -= other;
    return retval;
}

const BCInt BCInt::operator* (const BCInt& other) const
{
    BCInt retval;
    // implement
}

const BCInt BCInt::operator/ (const BCInt& other) const
{
    BCInt retval;
    // implement
}

const BCInt BCInt::operator% (const BCInt& other) const
{
    BCInt retval = *this;
    retval %= other;
    return retval;
}

BCInt& BCInt::operator+= (const BCInt& other)
{
    unsigned char carry = 0, myDigit, otherDigit;

    //auto myIter =
}

BCInt& BCInt::operator-= (const BCInt& other)
{
    // implement
}

BCInt& BCInt::operator*= (const BCInt& other)
{
    *this = *this * other;
    return *this;
}

BCInt& BCInt::operator/= (const BCInt& other)
{
    *this = *this / other;
    return *this;
}

BCInt& BCInt::operator%= (const BCInt& other)
{
    while (*this >= other)
    {
        *this -= other;
    }

    return *this;
}

bool BCInt::operator== (const BCInt& other) const
{
    return spaceship (other) == 0;
}

bool BCInt::operator!= (const BCInt& other) const
{
    return spaceship (other) != 0;
}

bool BCInt::operator< (const BCInt& other) const
{
    return spaceship (other) < 0;
}

bool BCInt::operator<= (const BCInt& other) const
{
    return spaceship (other) <= 0;
}

bool BCInt::operator> (const BCInt& other) const
{
    return spaceship (other) > 0;
}

bool BCInt::operator>= (const BCInt& other) const
{
    return spaceship (other) >= 0;
}

ostream& operator<< (ostream& out, const BCInt& value)
{
    out << value.to_string ();
    return out;
}

istream& operator>> (istream& in, BCInt& target)
{
    string buffer;

    if (in.peek () == '-' || in.peek () == '+')
    {
        buffer += in.get ();
    }

    while (isdigit (in.peek ()))
    {
        buffer += in.get ();
    }

    target = BCInt (buffer);

    return in;
}

int BCInt::sign () const
{
    if (digits.size () == 1 && digits.front () == 0b1111)
    {
        return 0;
    }
    else
    {
        return negative ? -1 : 1;
    }
}

BCInt::BCInt () :
    digits (), negative (false)
{}

int BCInt::spaceship (const BCInt& other) const
{
    if (sign () > other.sign ()) // n > -m, 0 > -m
    {
        return 1;
    }
    else if (sign () < other.sign ()) //-n < m, -n < 0
    {
        return -1;
    }
    else if (sign () == 0) // 0 == 0
    {
        return 0;
    }

    if (digits.size () > other.digits.size () ||
        (digits.size () == other.digits.size () && // I'm longer than other (also
                                                   // checks for terminal sequence)
         digits.front () >> 4 != 0b1111 && other.digits.front () >> 4 == 0b1111))
    {
        return negative ? -1 : 1;
    }
    else if (digits.size () < other.digits.size () ||
             (digits.size () == other.digits.size () && // I'm shorter than other
              digits.front () >> 4 == 0b1111 &&
              other.digits.front () >> 4 != 0b1111))
    {
        return negative ? 1 : -1;
    }

    auto meIter = cbegin ();
    auto themIter = other.cbegin ();

    while (meIter != cend ())
    {
        if (*meIter > *themIter)
        {
            return negative ? -1 : 1;
        }
        else if (*meIter < *themIter)
        {
            return negative ? 1 : -1;
        }
    }

    return 0;
}

BCInt::iterator BCInt::begin ()
{
    return iterator (true, digits.begin ());
}

BCInt::iterator BCInt::end ()
{
    if (digits.front () >> 4 == 0b1111)
    {
        return iterator (true, digits.end ());
    }
    else
    {
        return iterator (false, --digits.end ());
    }
}

BCInt::const_iterator BCInt::cbegin () const
{
    return const_iterator (true, digits.cbegin ());
}

BCInt::const_iterator BCInt::cend () const
{
    if (digits.front () >> 4 == 0b1111)
    {
        return const_iterator (true, digits.cend ());
    }
    else
    {
        return const_iterator (false, --digits.cend ());
    }
}