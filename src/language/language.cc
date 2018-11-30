// Copyright 2018 Justin Hu, Bronwyn Damm, Jacques Marais, Ramon Rakow, and Jude
// Sidloski
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

#include "language/exceptions/languageExceptions.h"

#include <algorithm>

namespace stacklang {
namespace {
using exceptions::StopError;
using exceptions::SyntaxError;
using exceptions::TypeError;
using stackelements::CommandElement;
using stackelements::DefinedCommandElement;
using stackelements::DefinedCommandPtr;
using stackelements::IdentifierElement;
using stackelements::IdentifierPtr;
using stackelements::PrimitiveCommandElement;
using stackelements::PrimitiveCommandPtr;
using stackelements::SubstackElement;
using stackelements::TypeElement;
using std::all_of;
using std::atomic_bool;
using std::string;
}  // namespace

atomic_bool stopFlag = false;

bool checkType(const StackElement* elm, const TypeElement& type) {
  if (elm == nullptr) {  // nullptr not matched ever.
    return false;
  } else if (type.getBase() == StackElement::DataType::Any &&
             type.getSpecialization() ==
                 nullptr) {  // any matches everything not null
    return true;
  } else if (type.getBase() != elm->getType()) {  // types don't match plainly
    return false;
  } else if (elm->getType() ==
             StackElement::DataType::Identifier) {  // identifier special case
    return dynamic_cast<const IdentifierElement*>(elm)->isQuoted();
  } else if (type.getSpecialization() == nullptr ||
             type.getSpecialization()->getBase() ==
                 StackElement::DataType::Any) {  // has no specialization or is
                                                 // an Any specialized substack.
    return true;                                 // type matches plainly
  } else if (elm->getType() == type.getBase() &&
             type.getBase() ==
                 StackElement::DataType::Substack) {  // is a specialized
                                                      // substack
    const Stack& s = dynamic_cast<const SubstackElement*>(elm)->getData();
    const TypeElement* spec = type.getSpecialization();

    return all_of(s.begin(), s.end(), [&spec](const StackElement* e) {
      return checkType(e, *spec);
    });
  } else if (elm->getType() == type.getBase() &&
             type.getBase() ==
                 StackElement::DataType::Command) {  // is a specialized command
    return (type.getSpecialization()->getBase() ==
            StackElement::DataType::Primitive) ==
           dynamic_cast<const CommandElement*>(elm)->isPrimitive();
  } else {  // is a specialized non-substack
    throw SyntaxError("Impossible type detected.", static_cast<string>(type),
                      0);
  }
}

void checkTypes(const Stack& s, const Stack& types) {
  auto typeIter = types.begin();
  auto stackIter = s.begin();

  for (; typeIter != types.end() && stackIter != s.end();
       typeIter++, stackIter++) {
    if (!checkType(*stackIter, *static_cast<const TypeElement*>(*typeIter))) {
      throw TypeError(**typeIter, **stackIter);
    }
  }
  if (typeIter != types.end()) {
    throw TypeError(**typeIter);
  }
}

void execute(Stack& s, Environment* env) {
  if (stopFlag) {
    stopFlag = false;
    throw StopError();
  }
  if (s.isEmpty()) return;

  if (s.top()->getType() == StackElement::DataType::Identifier &&
      !dynamic_cast<const IdentifierElement*>(s.top())
           ->isQuoted()) {  // identifier that isn't quoted.
    IdentifierPtr id(dynamic_cast<IdentifierElement*>(s.pop()));
    s.push(env->lookup(id->getName()));
    return execute(s, env);
  } else if (s.top()->getType() == StackElement::DataType::Command) {
    const CommandElement* cmd = dynamic_cast<const CommandElement*>(s.top());
    if (cmd->isPrimitive()) {
      PrimitiveCommandPtr prim(dynamic_cast<PrimitiveCommandElement*>(s.pop()));
      (*prim)(s, env);
    } else {
      DefinedCommandPtr func(dynamic_cast<DefinedCommandElement*>(s.pop()));
      (*func)(s);
    }
    return execute(s, env);
  }
}
}  // namespace stacklang
