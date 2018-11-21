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
using stacklang::stackelements::NumberElement;
using stacklang::stackelements::NumberPtr;
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
}  // namespace

atomic_bool stopFlag = false;

DefinedFunction::DefinedFunction(const Stack& sig, const Stack& b,
                                 const CommandElement* ctx) noexcept
    : signature(sig), body(b), context(ctx) {}

DefinedFunction::DefinedFunction() noexcept
    : signature(Stack{}), body(Stack{}), context(nullptr) {}

const Primitives& PRIMITIVES() noexcept {
  static const Primitives& prims = *new Primitives{
// Special include files to group definition of primitives.
#include "language/primitives/boolean.inc"
#include "language/primitives/command.inc"
#include "language/primitives/number.inc"
#include "language/primitives/special.inc"
#include "language/primitives/string.inc"
#include "language/primitives/substack.inc"
#include "language/primitives/type.inc"
  };
  return prims;
}

Environment& ENVIRONMENT() noexcept {
  static Environment& env = *new Environment{};
  return env;
}

// Look up the StackElement bound to the given identifier in the environment.
// It returns a pointer to the StackElement if found, and throws an error
// if not found.
//
// For reference, the Environment typedef is:
//    typedef std::vector<std::map<std::string, ElementPtr>> Environment;
ElementPtr lookupInEnv(const string& id, const Environment& env,
                       const vector<string>& context) {

  // start at back of vector and search through each map until id is found
  for (auto vecIterator = env.rbegin(); vecIterator != env.rend(); ++vecIterator) {

    std::map<std::string, ElementPtr>::iterator mapIterator;
    mapIterator = iterator->find(id);

    // once found, return the pointer to the StackElement
    if (mapIterator != vecIterator.end()) {
      return mapIterator->second->clone();
    }
  }
  // if not found, throw an error
  throw RuntimeError("Identifier " + *id + " doesn't exist.", context);
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

void checkContext(const string& actual, const CommandElement* required,
                  const string& name, const vector<string>& context) {
  if (required != nullptr) {
    if (actual == GLOBAL_CONTEXT) {
      throw SyntaxError("Attempted to use `" + name +
                            "` at top level. Expected to be within `" +
                            required->getName() + "`.",
                        context);
    } else if (actual != required->getName()) {
      throw SyntaxError("Attempted to use `" + name + "` within `" + actual +
                            "`. Expected to be within `" + required->getName() +
                            "`.",
                        context);
    }
  }
}

void execute(Stack& s, Environment& defines, vector<string> context) {
  if (stopFlag) {
    stopFlag = false;
    throw StopError(context);
  }
  if (s.isEmpty()) return;

  const auto& PRIMS = PRIMITIVES();

  // If it's an identifier, look it up in the environment and then push
  // the corresponding StackElement onto the stack. If it's not found,
  // the lookupInEnv() function will throw an error.
  if (s.top()->getType() == StackElement::DataType::Identifer) {

    ElementPtr identifier(dynamic_cast<IndentifierElement*>(s.pop()));
    ElementPtr identifierExpr(lookupInEnv(identifier->getName(), defines, context));
    s.push(identifierExpr);

  } else if (s.top()->getType() == StackElement::DataType::Command) {
    CommandPtr command(dynamic_cast<CommandElement*>(s.pop()));
    const auto& defResult =
        find_if(defines.begin(), defines.end(),
                [&command](const pair<string, DefinedFunction>& entry) {
                  return entry.first == command->getName();
                });  // find from non-primitives

    if (defResult != defines.end()) {  // It's defined!
      const auto& types = defResult->second.signature;
      const auto& commands = defResult->second.body;

      checkTypes(s, types, context);
      checkContext(context.back(), defResult->second.context,
                   command->getName(), context);

      context.push_back(command->getName());  // now executing function
      for (auto c : commands) {
        try {
          s.push(c->clone());
        } catch (const StackOverflowError& e) {
          // stack error without trace must be main stack.
          if (e.getTrace().empty())
            throw StackOverflowError(s.getLimit(), context);
          else
            throw e;
        } catch (const StackUnderflowError& e) {
          if (e.getTrace().empty())
            throw StackUnderflowError(context);
          else
            throw e;
        }
        execute(s, defines, context);  // TODO: make this tail recursive.
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
        context.push_back(primResult->first);
        try {
          primResult->second.second(
              s, defines,
              context);  // note that any errors from main stack
                         // interaction need to have stacktrace added.
        } catch (const StackOverflowError& e) {
          // see above.
          if (e.getTrace().empty())
            throw StackOverflowError(s.getLimit(), context);
        } catch (const StackUnderflowError& e) {
          if (e.getTrace().empty()) throw StackUnderflowError(context);
        }
        context.pop_back();
        return execute(
            s, defines,
            context);  // clear off any commands produced but not executed
      } else {
        throw SyntaxError("Given command is not recognized.",
                          command->getName(), 0, context);
      }
    }
  }
}
}  // namespace stacklang
