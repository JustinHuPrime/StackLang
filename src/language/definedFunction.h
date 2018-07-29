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

// Struct for a defined function - just a way to group things

#ifndef STACKLANG_LANGUAGE_DEFINEDFUNCTION_H_
#define STACKLANG_LANGUAGE_DEFINEDFUNCTION_H_

#include "language/stack.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/typeElement.h"

#include <list>

namespace stacklang {
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::TypeElement;
using std::list;

struct DefinedFunction {
  const list<TypeElement>& signature;
  const Stack& body;
  const CommandElement* context;
  DefinedFunction(const list<TypeElement>& sig, const Stack& b,
                  const CommandElement* ctx = nullptr) noexcept;
};
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_DEFINEDFUNCTION_H_