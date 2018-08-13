// Copyright 2018 Justin Hu
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

// Various math utility functions

#ifndef STACKLANG_UTILS_MATHUTILS_H_
#define STACKLANG_UTILS_MATHUTILS_H_

namespace util {
int spaceship(long double a, long double b, long double delta) noexcept;
long double significantDelta(long double a, long double b) noexcept;
}  // namespace util
#endif  // STACKLANG_UTILS_MATHUTILS_H_