// Copyright 2018 Justin Hu, Bronwyn Damm, Jacques Marais, Ramon Rakow, and Jude
// Sidloski
//
// This file is part of the StackLang interpreter.
//
// The StackLang interpreter is free software: you can redistribute it and/or
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
// Produce true if first string starts/ends with the second string
bool starts_with(const std::string& outer, const std::string& prefix) noexcept;
bool ends_with(const std::string& outer, const std::string& suffix) noexcept;

// Escapes any \, ", or newline into a \\, \", or \n
std::string escape(std::string) noexcept;

// Unescapes string - reverse of previous function
std::string unescape(std::string) noexcept;

// Returns the position of any improper escape (\ escaping anything but a \, ",
// or n), or the posiiton of an unescaped \ or ". If none found, returns
// string::npos
size_t findImproperEscape(const std::string&) noexcept;

// produces 'n' spaces in a row
std::string spaces(size_t) noexcept;

// removes all occurences of character c from string s
std::string removeChar(std::string, char) noexcept;

// trims any whitespace from the start and the end of the string
std::string trim(std::string) noexcept;

const char WHITESPACE[] = " \t\n\r";
}  // namespace util

#endif  // STACKLANG_UTILS_STRINGUTILS_H_
