#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <stdexcept>
#include <string>
#include <utility>
using std::invalid_argument;
using std::pair;
using std::string;

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
string spaces (unsigned);

/**
 * removes all occurences of character c from string s
 */
string removeChar (string, char);

#endif /* STRINGUTILS_H */