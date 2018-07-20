#ifndef STACKLANG_UTILS_STRINGUTILS_H_
#define STACKLANG_UTILS_STRINGUTILS_H_

#include <string>

namespace Util {
using std::string;

// Produce true if first string starts/ends with the second string
bool starts_with(const string&, const string&) noexcept;
bool ends_with(const string&, const string&) noexcept;

// Escapes any \, ", or newline into a \\, \", or \n
string escape(string) noexcept;

// Unescapes string - reverse of previous function
string unescape(string) noexcept;

// Returns the position of any improper escape (\ escaping anything but a \, ",
// or n), or the posiiton of an unescaped \ or ". If none found, returns
// string::npos
size_t findImproperEscape(const string&) noexcept;

// produces 'n' spaces in a row
string spaces(unsigned) noexcept;

// removes all occurences of character c from string s
string removeChar(string, char) noexcept;

// trims any whitespace from the start and the end of the string
string trim(string) noexcept;

const char WHITESPACE[] = " \t\n\r";
}  // namespace Util

#endif  // STACKLANG_UTILS_STRINGUTILS_H_