#ifndef BIGDECIMAL_H
#define BIGDECIMAL_H

#include <list>
#include <iostream>
using std::ostream;
using std::istream;
using std::list;
using std::string;

/**
 * Represents an arbitrary precision decimal number using base 65530.
 */
class BigDecimal
{
    public:

    /**
     * Takes a string representing a big decimal (assumes string is a properly-formatted decimal)
     */
    BigDecimal (const string&);

    /**
     * string conversion
     */
    operator string () const;
    
    /**
     * Sign operators
     */
    const BigDecimal operator+ () const;
    const BigDecimal operator- () const;

    /**
     * increment and decrement
     */
    BigDecimal& operator++ (); //pre
    BigDecimal operator++ (int); //post
    BigDecimal& operator-- ();
    BigDecimal operator-- (int);

    /**
     * Arithmetic operators
     */
    const BigDecimal operator+ (const BigDecimal&) const;
    const BigDecimal operator- (const BigDecimal&) const;
    const BigDecimal operator* (const BigDecimal&) const;
    const BigDecimal operator/ (const BigDecimal&) const;
    const BigDecimal operator% (const BigDecimal&) const;

    /**
     * Compound arithmetic operators
     */
    BigDecimal& operator+= (const BigDecimal&);
    BigDecimal& operator-= (const BigDecimal&);
    BigDecimal& operator*= (const BigDecimal&);
    BigDecimal& operator/= (const BigDecimal&);
    BigDecimal& operator%= (const BigDecimal&);

    /**
     * Comparison operators
     */
    bool operator== (const BigDecimal&) const;
    bool operator!= (const BigDecimal&) const;
    bool operator< (const BigDecimal&) const;
    bool operator<= (const BigDecimal&) const;
    bool operator> (const BigDecimal&) const;
    bool operator>= (const BigDecimal&) const;

    /**
     * Stream output and input operators
     */
    friend ostream& operator<< (ostream&, const BigDecimal&);
    friend istream& operator>> (istream&, const BigDecimal&);

    private:

    list<unsigned> wholePart;
    list<unsigned> decimalPart;
    bool negative;
};

#endif