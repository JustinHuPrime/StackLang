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

// Stack iterator

#ifndef STACKLANG_LANGUAGE_STACK_STACKITERATOR_H_
#define STACKLANG_LANGUAGE_STACK_STACKITERATOR_H_

#include "language/stack/stackElement.h"
#include "language/stackImpl.h"

namespace stacklang {
using stacklang::Stack;
using stacklang::StackElement;

class Stack::StackIterator {
 public:
  typedef size_t difference_type;
  typedef StackElement* value_type;
  typedef StackElement** pointer;
  typedef StackElement*& reference;
  typedef size_t size_type;
  typedef std::input_iterator_tag iterator_category;

  StackIterator(Stack::Node*) noexcept;
  StackIterator(const StackIterator&) noexcept = default;

  StackIterator& operator=(const StackIterator&) noexcept = default;

  friend bool operator==(const Stack::StackIterator&,
                         const Stack::StackIterator&) noexcept;
  friend bool operator!=(const Stack::StackIterator&,
                         const Stack::StackIterator&) noexcept;

  const StackElement* operator*() noexcept;

  StackIterator& operator++() noexcept;
  StackIterator operator++(int) noexcept;

 private:
  Stack::Node* curr;
};

bool operator==(const Stack::StackIterator&,
                const Stack::StackIterator&) noexcept;
bool operator!=(const Stack::StackIterator&,
                const Stack::StackIterator&) noexcept;
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKITERATOR_H_