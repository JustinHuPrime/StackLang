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

// Stack iterator definition. Uses unifying include for stack parts

#include "language/stack.h"

namespace stacklang {
using stacklang::Stack;
using stacklang::StackElement;

Stack::StackIterator::StackIterator(Stack::Node* node) noexcept : curr(node) {}

const StackElement* Stack::StackIterator::operator*() noexcept {
  return curr->elm.get();
}

Stack::StackIterator& Stack::StackIterator::operator++() noexcept {
  if (curr != nullptr) {
    curr = curr->next;
  }

  return *this;
}

Stack::StackIterator Stack::StackIterator::operator++(int) noexcept {
  StackIterator temp = *this;

  operator++();

  return temp;
}

bool operator==(const Stack::StackIterator& first,
                const Stack::StackIterator& second) noexcept {
  return first.curr == second.curr;
}

bool operator!=(const Stack::StackIterator& first,
                const Stack::StackIterator& second) noexcept {
  return !(first == second);
}
}  // namespace stacklang