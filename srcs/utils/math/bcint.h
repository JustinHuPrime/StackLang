#ifndef BCINT_H
#define BCINT_H

#include <iostream>
#include <list>
#include <string>
using std::istream;
using std::list;
using std::ostream;
using std::string;

/**
 * A 0xf terminated binary-coded decimal integer
 */
class BCInt
{
  public:
    /**
     * Constructs from either an integral number, or a string of numbers with an optional sign.
     */
    BCInt (long);
    BCInt (const string&);

    /**
     * string conversion (must be explicitly called)
     */
    string to_string () const;

    /**
     * Sign operators
     */
    const BCInt operator+ () const;
    const BCInt operator- () const;

    /**
     * increment and decrement
     */
    BCInt& operator++ ();   //pre
    BCInt operator++ (int); //post
    BCInt& operator-- ();
    BCInt operator-- (int);

    /**
     * Arithmetic operators
     */
    const BCInt operator+ (const BCInt&) const;
    const BCInt operator- (const BCInt&) const;
    const BCInt operator* (const BCInt&) const;
    const BCInt operator/ (const BCInt&) const;
    const BCInt operator% (const BCInt&) const;

    /**
     * Compound arithmetic operators
     */
    BCInt& operator+= (const BCInt&);
    BCInt& operator-= (const BCInt&);
    BCInt& operator*= (const BCInt&);
    BCInt& operator/= (const BCInt&);
    BCInt& operator%= (const BCInt&);

    /**
     * Comparison operators
     */
    bool operator== (const BCInt&) const;
    bool operator!= (const BCInt&) const;
    bool operator< (const BCInt&) const;
    bool operator<= (const BCInt&) const;
    bool operator> (const BCInt&) const;
    bool operator>= (const BCInt&) const;

    /**
     * Stream output and input operators
     */
    friend ostream& operator<< (ostream&, const BCInt&);
    friend istream& operator>> (istream&, BCInt&);

    /**
     * getters
     */
    int sign () const;

  private:
    class iterator;
    class const_iterator;

    list< unsigned char > digits;
    bool negative;

    BCInt ();

    iterator begin ();
    iterator end ();
    const_iterator cbegin () const;
    const_iterator cend () const;

    int spaceship (const BCInt&) const;
};

#endif /* BCINT_H */