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

// Implementation of math utility functions

#include "util/mathUtils.h"

#include <cmath>

namespace util {
namespace {}
int spaceship(long double a, long double b, long double delta) noexcept {
  long double difference = a - b;
  if (fabs(difference) < delta)
    return 0;
  else if (difference > 0)
    return -1;
  else
    return 1;
}
}  // namespace util