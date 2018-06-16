#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <utility>
#include <stdexcept>
using std::string;
using std::invalid_argument;
using std::pair;

/**
 * Produce true if first string starts/ends with the second string
 */
bool starts_with (const string&, const string&);
bool ends_with (const string&, const string&);

/**
 * Escapes any \, ", or newline into a \\, \", or \n
 */
string escape (string);

/**
 * Unescapes string - reverse of previous function
 */
string unescape (string);

/**
 * Returns true if all \'es escape a \, ", or n, and doesn't have any unescaped \ or "
 */
size_t findImproperEscape (const string&);

/**
 * produces 'n' spaces in a row
 */
string spaces (int);

#endif /* STRINGUTILS_H */