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

// Tests for primitive command operations

#include "catch.hpp"
#include "language/exceptions/languageExceptions.h"
#include "language/language.h"
#include "language/stack/stack.h"
#include "language/stack/stackElements.h"

namespace {
using stacklang::Defines;
using stacklang::ElementPtr;
using stacklang::execute;
using stacklang::Stack;
using stacklang::StackElement;
using stacklang::exceptions::RuntimeError;
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

Defines emptyDefines;
}  // namespace

TEST_CASE("command? primitive true case", "[primitives][Command][command?]") {
  Stack s{new CommandElement("foldr"), new CommandElement("command?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("command? primitive false case", "[primitives][Command][command?]") {
  Stack s{new StringElement("I'm a command - trust me!"),
          new CommandElement("command?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("quoted? primitive true case", "[primitives][Command][quoted?]") {
  Stack s{new CommandElement("foldr", true), new CommandElement("quoted?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("quoted? primitive simple false case",
          "[primitives][Command][quoted?]") {
  Stack s{new StringElement("I'm a quoted command - trust me!"),
          new CommandElement("quoted?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("quoted? primitive complex false case",
          "[primitives][Command][quoted?]") {
  Stack s{new CommandElement("foldr", false), new CommandElement("quoted?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("defined? false case", "[primtives][Command][defined?]") {
  Stack s{new CommandElement("doesnt-exist"), new CommandElement("defined?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("defined? primtive true case", "[primtives][Command][defined?]") {
  Stack s{new CommandElement("add"), new CommandElement("defined?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("defined? defined true case", "[primtives][Command][defined?]") {
  Stack s{
      new SubstackElement(Stack{}),
      new SubstackElement(Stack{}),
      new CommandElement("function-name", true),
      new CommandElement("define", false),
  };
  Defines d;
  execute(s, d);
  s.push(new CommandElement("function-name"));
  s.push(new CommandElement("defined?"));
  execute(s, d);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("local? not found", "[primitives][Command][local?]") {}
TEST_CASE("local? primitive", "[primitives][Command][local?]") {}
TEST_CASE("local? defined local", "[primitives][Command][local?]") {}
TEST_CASE("local? defined global", "[primitives][Command][local?]") {}

TEST_CASE("unquote quoted", "[primitives][Command][local?]") {}

TEST_CASE("command-to-string", "[primitives][Command][command-to-string]") {}
TEST_CASE("string-to-command", "[primitives][Command][string-to-command]") {}
TEST_CASE("string-to-command, coerce to quoted",
          "[primitives][Command][string-to-command*]") {}
TEST_CASE("string-to-command*, already quoted",
          "[primitives][Command][string-to-command*]") {}

TEST_CASE("arity not found", "[primitives][Command][arity]") {}
TEST_CASE("arity primitive", "[primitives][Command][arity]") {}
TEST_CASE("arity defined local", "[primitives][Command][arity]") {}

TEST_CASE("body not found", "[primitives][Command][body]") {}
TEST_CASE("body primitive", "[primitives][Command][body]") {}
TEST_CASE("body defined local", "[primitives][Command][body]") {}

TEST_CASE("context not found", "[primitives][Command][context]") {}
TEST_CASE("context primitive", "[primitives][Command][context]") {}
TEST_CASE("context defined local", "[primitives][Command][context]") {}

TEST_CASE("signature not found", "[primitives][Command][signature]") {}
TEST_CASE("signature primitive", "[primitives][Command][signature]") {}
TEST_CASE("signature defined local", "[primitives][Command][signature]") {}