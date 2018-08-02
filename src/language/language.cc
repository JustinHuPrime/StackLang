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

// Implementation of the core language evaluation system.

#include "language/language.h"

#include <algorithm>
#include <iterator>

#include <gmp.h>
#include <gmpxx.h>

#include "language/exceptions/languageExceptions.h"
#include "language/stack/stack.h"

namespace stacklang {
namespace {
using stacklang::exceptions::StopError;
using stacklang::exceptions::SyntaxError;
using stacklang::exceptions::TypeError;
using stacklang::stackelements::BooleanElement;
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::NumberElement;
using stacklang::stackelements::StringElement;
using stacklang::stackelements::SubstackElement;
using stacklang::stackelements::TypeElement;
using std::all_of;
using std::begin;
using std::end;
using std::find_if;
}  // namespace

bool stopFlag = false;

DefinedFunction::DefinedFunction(const Stack& sig, const Stack& b,
                                 const CommandElement* ctx) noexcept
    : signature(sig), body(b), context(ctx) {}

const map<string, PrimitiveFunction>& PRIMITIVES() noexcept {
  static map<string, PrimitiveFunction>* prims =
      new map<string, PrimitiveFunction>{
// Special include files to group definition of primitives.
#include "language/primitives/boolean.inc"
#include "language/primitives/command.inc"
#include "language/primitives/number.inc"
#include "language/primitives/special.inc"
#include "language/primitives/stdlib.inc"
#include "language/primitives/string.inc"
#include "language/primitives/substack.inc"
#include "language/primitives/type.inc"
      };
  return *prims;
}

bool checkType(const StackElement* elm, const TypeElement type,
               const list<CommandElement*>& context) {
  if (elm == nullptr) {  // nullptr not matched ever.
    return false;
  } else if (type.getData() == StackElement::DataType::Any &&
             type.getSpecialization() ==
                 nullptr) {  // any matches everything not null
    return true;
  } else if (type.getSpecialization() == nullptr ||
             type.getSpecialization()->getData() ==
                 StackElement::DataType::Any) {  // has no specialization or is
                                                 // an Any specialized substack.
    return elm->getType() == type.getData();     // type matches plainly
  } else if (elm->getType() == type.getData() &&
             type.getData() ==
                 StackElement::DataType::Number) {  // is a specialized number
    return type.getSpecialization()->getData() ==
           (static_cast<const NumberElement*>(elm)->isExact()
                ? StackElement::DataType::Exact
                : StackElement::DataType::Inexact);
  } else if (elm->getType() == type.getData() &&
             type.getData() ==
                 StackElement::DataType::Command) {  // is a specialized command
    return type.getSpecialization()->getData() ==
           (static_cast<const CommandElement*>(elm)->isQuoted()
                ? StackElement::DataType::Quoted
                : StackElement::DataType::Unquoted);
  } else if (elm->getType() == type.getData() &&
             type.getData() ==
                 StackElement::DataType::Substack) {  // is a specialized
                                                      // substack
    const Stack& s = static_cast<const SubstackElement*>(elm)->getData();
    const TypeElement* spec = type.getSpecialization();

    return all_of(s.begin(), s.end(), [&spec, &context](const StackElement* e) {
      return checkType(e, *spec, context);
    });
  } else {  // is a specialized non-substack, non-number
    throw new SyntaxError("Impossible type detected.",
                          static_cast<string>(type), 0, context);
  }
}

void checkTypes(const Stack& s, const Stack& types,
                const list<CommandElement*>& context) {
  auto typeIter = types.begin();
  auto stackIter = s.begin();

  for (; typeIter != types.end() && stackIter != s.end();
       typeIter++, stackIter++) {
    if (!checkType(*stackIter, *static_cast<const TypeElement*>(*typeIter),
                   context)) {
      throw TypeError(**stackIter, **typeIter, context);
    }
  }
  if (typeIter != types.end()) {
    throw TypeError(**typeIter, context);
  }
}

void checkContext(const CommandElement* actual, const CommandElement* required,
                  const string& name, const list<CommandElement*>& context) {
  if (required != nullptr) {
    if (actual == nullptr) {
      throw SyntaxError("Attempted to use `" + name +
                            "` at top level. Expected to be within `" +
                            required->getName() + "`.",
                        context);
    } else if (actual->getName() != required->getName()) {
      throw SyntaxError("Attempted to use `" + name + "` within `" +
                            actual->getName() + "`. Expected to be within `" +
                            required->getName() + "`.",
                        context);
    }
  }
}

void execute(Stack& s, map<string, DefinedFunction>& defines,
             list<CommandElement*> context) {
  if (stopFlag) {
    stopFlag = false;
    throw StopError();
  }

  const auto& PRIMS = PRIMITIVES();

  if (s.top()->getType() == StackElement::DataType::Command &&
      !dynamic_cast<CommandElement*>(s.top())->isQuoted()) {
    CommandElement* command = dynamic_cast<CommandElement*>(s.pop());
    const auto& defResult =
        find_if(defines.begin(), defines.end(),
                [&command](const pair<string, DefinedFunction>& entry) {
                  return entry.first == command->getName();
                });  // find from non-primitives

    if (defResult != defines.end()) {  // It's defined!
      const auto& types = defResult->second.signature;
      const auto& commands = defResult->second.body;

      checkTypes(s, types, context);
      checkContext(context.front(), defResult->second.context,
                   command->getName(), context);

      context.push_back(command);  // now executing function
      for (auto c : commands) {
        s.push(c->clone());
        execute(s, defines, context);
      }
      context.pop_back();  // done with function
      return execute(
          s, defines,
          context);  // clear off any commands produced but not executed
    } else {         // It's a primitive (maybe?)
      const auto& primResult =
          find_if(PRIMS.begin(), PRIMS.end(),
                  [&command](const pair<string, PrimitiveFunction>& entry) {
                    return entry.first == command->getName();
                  });  // check the primitives

      if (primResult != PRIMS.end()) {
        const auto& types = primResult->second.first;

        checkTypes(s, types, context);

        primResult->second.second(s, defines);
        return execute(
            s, defines,
            context);  // clear off any commands produced but not executed
      } else {
        throw SyntaxError("Given command is not recognized.",
                          command->getName(), 0, context);
      }
    }

    delete command;
  }
}  // namespace stacklang
}  // namespace stacklang