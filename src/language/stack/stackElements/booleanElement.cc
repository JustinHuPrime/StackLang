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

#include "language/stack/stackElements/booleanElement.h"

namespace stacklang {
namespace stackelements {
const char* BooleanElement::TSTR = "true";
const char* BooleanElement::FSTR = "false";

BooleanElement::BooleanElement(bool b) noexcept
    : StackElement(StackElement::DataType::Boolean), data(b) {}

BooleanElement* BooleanElement::clone() const noexcept {
  return new BooleanElement(data);
}

BooleanElement::operator string() const noexcept { return data ? TSTR : FSTR; }

bool BooleanElement::getData() const noexcept { return data; }
}  // namespace stackelements
}  // namespace stacklang