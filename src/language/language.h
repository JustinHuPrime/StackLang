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
#include <list>
#include <map>
#include <utility>

#include "language/stack/stack.h"
#include "language/stack/stackElements.h"

namespace stacklang {
namespace {
using stackelements::CommandElement;
using stackelements::TypeElement;
using std::atomic_bool;
using std::list;
using std::map;
using std::pair;
}  // namespace

struct DefinedFunction {
  Stack signature;
  Stack body;
  const CommandElement* context;
  DefinedFunction(const Stack& sig, const Stack& b,
                  const CommandElement* ctx = nullptr) noexcept;
  DefinedFunction() noexcept;
};

typedef map<string, DefinedFunction> Defines;
typedef void (*const Prim)(Stack&, Defines&, list<string>&);
typedef pair<Stack, Prim> PrimitiveFunction;
typedef map<string, PrimitiveFunction> Primitives;

const Primitives& PRIMITIVES() noexcept;
const char* const GLOBAL_CONTEXT = "global context";

bool checkType(const StackElement* elm, const TypeElement type,
               const list<string>& context);
void checkTypes(const Stack& s, const Stack& types,
                const list<string>& context);
void checkContext(const string& actual, const CommandElement* required,
                  const string& name, const list<string>& context);
void execute(Stack&, Defines&,
             list<string> = list<string>{
                 GLOBAL_CONTEXT});  // Executes the stack until it
                                    // encounters a data element

extern atomic_bool stopFlag;  // signal handlers set this to stop execution.
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_