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

#define _USE_MATH_DEFINES

#include "language/language.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <random>
#include <sstream>
#include <stack>

#include "language/exceptions/interpreterExceptions.h"
#include "language/exceptions/languageExceptions.h"
#include "language/stack/stack.h"
#include "util/mathUtils.h"
#include "util/stringUtils.h"

namespace stacklang {
namespace {
using stacklang::exceptions::ParserException;
using stacklang::exceptions::RuntimeError;
using stacklang::exceptions::StackOverflowError;
using stacklang::exceptions::StackUnderflowError;
using stacklang::exceptions::StopError;
using stacklang::exceptions::SyntaxError;
using stacklang::exceptions::TypeError;
using stacklang::stackelements::BooleanElement;
using stacklang::stackelements::BooleanPtr;
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::CommandPtr;
using stacklang::stackelements::DefinedCommandElement;
using stacklang::stackelements::DefinedCommandPtr;
using stacklang::stackelements::IdentiferPtr;
using stacklang::stackelements::IdentifierElement;
using stacklang::stackelements::NumberElement;
using stacklang::stackelements::NumberPtr;
using stacklang::stackelements::PrimitiveCommandElement;
using stacklang::stackelements::PrimitiveCommandPtr;
using stacklang::stackelements::StringElement;
using stacklang::stackelements::StringPtr;
using stacklang::stackelements::SubstackElement;
using stacklang::stackelements::SubstackPtr;
using stacklang::stackelements::TypeElement;
using stacklang::stackelements::TypePtr;
using std::abs;
using std::acosh;
using std::all_of;
using std::asinh;
using std::atanh;
using std::atomic_bool;
using std::begin;
using std::copysign;
using std::cos;
using std::cosh;
using std::end;
using std::find_if;
using std::ifstream;
using std::isnan;
using std::istringstream;
using std::log;
using std::make_unique;
using std::max;
using std::min;
using std::modf;
using std::pair;
using std::pow;
using std::random_device;
using std::sin;
using std::sinh;
using std::stack;
using std::string;
using std::tan;
using std::tanh;
using std::to_string;
using std::vector;
using util::ends_with;
using util::spaceship;
using util::starts_with;
using util::trim;

StackElement* getOrError(const Environment& env, const string& id,
                         const vector<string>& st) {
  for (const auto& layer : env) {
    auto iter = layer.find(id);
    if (iter != layer.cend()) {
      return iter->second->clone();
    }
  }

  throw RuntimeError("Cannot find identifier '" + id + "'.", st);
}
}  // namespace

atomic_bool stopFlag = false;

DefinedFunction::DefinedFunction(const Stack& sig, const Stack& b,
                                 const CommandElement* ctx) noexcept
    : signature(sig), body(b), context(ctx) {}

DefinedFunction::DefinedFunction() noexcept
    : signature(Stack{}), body(Stack{}), context(nullptr) {}

Environment& ENVIRONMENT() noexcept {
  static Environment& env = *new Environment{};
  return env;
}

bool checkType(const StackElement* elm, const TypeElement& type,
               const vector<string>& context) {
  if (elm == nullptr) {  // nullptr not matched ever.
    return false;
  } else if (type.getBase() == StackElement::DataType::Any &&
             type.getSpecialization() ==
                 nullptr) {  // any matches everything not null
    return true;
  } else if (type.getBase() != elm->getType()) {  // types don't match plainly
    return false;
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

    return all_of(s.begin(), s.end(), [&spec, &context](const StackElement* e) {
      return checkType(e, *spec, context);
    });
  } else {  // is a specialized non-substack
    throw SyntaxError("Impossible type detected.", static_cast<string>(type), 0,
                      context);
  }
}

void checkTypes(const Stack& s, const Stack& types,
                const vector<string>& context) {
  auto typeIter = types.begin();
  auto stackIter = s.begin();

  for (; typeIter != types.end() && stackIter != s.end();
       typeIter++, stackIter++) {
    if (!checkType(*stackIter, *static_cast<const TypeElement*>(*typeIter),
                   context)) {
      throw TypeError(**typeIter, **stackIter, context);
    }
  }
  if (typeIter != types.end()) {
    throw TypeError(**typeIter, context);
  }
}

void execute(Stack& s, Environment& env, vector<string> st) {
  if (stopFlag) {
    stopFlag = false;
    throw StopError(st);
  }
  if (s.isEmpty()) return;

  if (s.top()->getType() == StackElement::DataType::Identifier &&
      !dynamic_cast<const IdentifierElement*>(s.top())
           ->isQuoted()) {  // identifier that isn't quoted.
    IdentiferPtr id(dynamic_cast<IdentifierElement*>(s.pop()));
    s.push(getOrError(env, id->getName(), st));
  } else if (s.top()->getType() == StackElement::DataType::Command) {
    const CommandElement* cmd = dynamic_cast<const CommandElement*>(s.top());
    if (cmd->isPrimitive()) {
      PrimitiveCommandPtr prim(dynamic_cast<PrimitiveCommandElement*>(s.pop()));
      (*prim)(s, env, st);
    } else {
      DefinedCommandPtr func(dynamic_cast<DefinedCommandElement*>(s.pop()));
      (*func)(s, st);
    }
  }
}
}  // namespace stacklang
