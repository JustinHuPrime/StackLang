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

// environment tree implementation

#define _USE_MATH_DEFINES

#include "language/environment.h"

#include "language/exceptions/interpreterExceptions.h"
#include "language/exceptions/languageExceptions.h"
#include "language/language.h"
#include "language/stack/stackElements.h"
#include "util/mathUtils.h"
#include "util/stringUtils.h"

#include <cmath>
#include <fstream>
#include <random>
#include <stack>

#define PRIMDEF(name, body) \
  {name, new PrimitiveCommandElement([](Stack & s, Environment * e) body)},

namespace stacklang {
namespace {
using exceptions::RuntimeError;
using stackelements::PrimitiveCommandElement;
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
using stacklang::stackelements::IdentifierElement;
using stacklang::stackelements::IdentifierPtr;
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
using std::map;
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

EnvTree::Environment::Environment(Environment* p) noexcept : parent{p} {
  if (parent != nullptr) parent->children.push_back(this);
}

EnvTree::Environment::~Environment() noexcept {
  for (auto& child : children) delete child;
  clearBindings();
}

StackElement* EnvTree::Environment::lookup(const string& id) {
  auto iter = bindings.find(id);
  if (iter == bindings.cend()) {
    if (parent == nullptr) {
      throw RuntimeError("Cannot find identifier '" + id + "'.");
    } else {
      return parent->lookup(id);
    }
  } else {
    return iter->second->clone();
  }
}

void EnvTree::Environment::clearBindings() noexcept {
  for (auto& elm : bindings) delete elm.second;
}

EnvTree::EnvTree() noexcept {
  root = new Environment(nullptr);
  root->bindings = map<string, StackElement*>{
#include "language/primitives/boolean.inc"
#include "language/primitives/command.inc"
#include "language/primitives/number.inc"
#include "language/primitives/special.inc"
#include "language/primitives/string.inc"
#include "language/primitives/substack.inc"
#include "language/primitives/type.inc"
  };
}

EnvTree::~EnvTree() noexcept { delete root; }

Environment* EnvTree::getRoot() noexcept { return root; }
}  // namespace stacklang