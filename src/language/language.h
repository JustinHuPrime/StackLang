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

#include <atomic>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "language/stack/stack.h"
#include "language/stack/stackElements.h"

namespace stacklang {
Environment& GLOBAL_ENVIRONMENT() noexcept;

bool checkType(const StackElement* elm, const stackelements::TypeElement& type);
void checkTypes(const Stack& s, const Stack& types);
void execute(Stack&, Environment&);  // Executes the stack until it
                                     // encounters a data element

extern std::atomic_bool
    stopFlag;  // signal handlers set this to stop execution.
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_
