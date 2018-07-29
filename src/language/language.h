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

// The core of the language

#ifndef STACKLANG_LANGUAGE_LANGUAGE_H_
#define STACKLANG_LANGUAGE_LANGUAGE_H_

#include <array>
#include <list>
#include <map>
#include <utility>

#include "language/definedFunction.h"
#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/typeElement.h"

namespace stacklang {
using stackelements::CommandElement;
using stackelements::TypeElement;
using std::array;
using std::list;
using std::map;
using std::pair;

typedef void (*const Prim)(Stack&, map<string, DefinedFunction>&);
typedef pair<list<TypeElement>, Prim> PrimitiveFunction;

const map<string, PrimitiveFunction>& PRIMITIVES() noexcept;
bool checkType(const StackElement*, const TypeElement) noexcept;
void checkTypes(const Stack&, const list<TypeElement>&);
void execute(Stack&, map<string, DefinedFunction>&,
             list<CommandElement*> = list<CommandElement*>{
                 nullptr});  // Executes the stack until it
                             // encounters a data element

extern bool stopFlag;  // signal handlers set this to stop execution.
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_