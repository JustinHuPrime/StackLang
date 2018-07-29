// Copyright 2018 Justin Hu
//
// This file is part of the StackLang interpreter.
//
// The StackLang interpreter is free software: you can redistribute it and / or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The StackLang interpreter is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// the StackLang interpreter.  If not, see <https://www.gnu.org/licenses/>.

// Various string utility functions

#ifndef STACKLANG_UTILS_STRINGUTILS_H_
#define STACKLANG_UTILS_STRINGUTILS_H_

#include <string>

namespace util {
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
}  // namespace util

#endif  // STACKLANG_UTILS_STRINGUTILS_H_