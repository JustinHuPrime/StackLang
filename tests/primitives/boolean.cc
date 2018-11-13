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

// Tests for primitive boolean operations

#include "catch.hpp"
#include "language/exceptions/languageExceptions.h"
#include "language/language.h"
#include "language/stack/stack.h"
#include "language/stack/stackElements.h"

namespace {
using stacklang::Defines;
using stacklang::ElementPtr;
using stacklang::Stack;
using stacklang::StackElement;
using stacklang::exceptions::RuntimeError;
using stacklang::execute;
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

TEST_CASE("boolean? primitive true case", "[primitives][Boolean][boolean?]") {
  Stack s{new BooleanElement(true), new CommandElement("boolean?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("boolean? primitive false case", "[primitives][Boolean][boolean?]") {
  Stack s{new StringElement("I'm a boolean - trust me!"),
          new CommandElement("boolean?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("false? primitive true case", "[primitives][Boolean][false?]") {
  Stack s{new BooleanElement(false), new CommandElement("false?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("false? primitive simple false case",
          "[primitives][Boolean][false?]") {
  Stack s{new StringElement("I'm a `false` - trust me!"),
          new CommandElement("false?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("false? primitive complex false case",
          "[primitives][Boolean][false?]") {
  Stack s{new BooleanElement(true), new CommandElement("false?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("true? primitive true case", "[primitives][Boolean][true?]") {
  Stack s{new BooleanElement(true), new CommandElement("true?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("true? primitive simple false case", "[primitives][Boolean][true?]") {
  Stack s{new StringElement("I'm a `true` - trust me!"),
          new CommandElement("true?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("true? primitive complex false case",
          "[primitives][Boolean][true?]") {
  Stack s{new BooleanElement(false), new CommandElement("true?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("boolean-to-string simple test",
          "[primitives][Boolean][boolean-to-string]") {
  Stack s{new BooleanElement(false), new CommandElement("boolean-to-string")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::String);
  REQUIRE(dynamic_cast<StringElement*>(ptr.get())->getData() == "false");
}

TEST_CASE("string-to-boolean simple case",
          "[primitives][Boolean][string-to-boolean]") {
  Stack s{new StringElement("false"), new CommandElement("string-to-boolean")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(ptr->getType() == StackElement::DataType::Boolean);
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("string-to-boolean fail case",
          "[primitives][Boolean][string-to-boolean]") {
  Stack s{new StringElement("\"I'm really a string!\""),
          new CommandElement("string-to-boolean")};
  REQUIRE_THROWS_AS(execute(s, emptyDefines), RuntimeError);
}

TEST_CASE("if true case", "[primitive][Boolean][if]") {
  Stack s{new StringElement("true case"), new StringElement("false case"),
          new BooleanElement(true), new CommandElement("if")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<StringElement*>(ptr.get())->getData() == "true case");
}

TEST_CASE("if false case", "[primitive][Boolean][if]") {
  Stack s{new StringElement("true case"), new StringElement("false case"),
          new BooleanElement(false), new CommandElement("if")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<StringElement*>(ptr.get())->getData() == "false case");
}

TEST_CASE("not simple test", "[primitive][Boolean][not]") {
  Stack s{new BooleanElement(false), new CommandElement("not")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("or true case", "[primitive][Boolean][or]") {
  Stack s{new BooleanElement(false), new BooleanElement(true),
          new CommandElement("or")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("or false case", "[primitive][Boolean][or]") {
  Stack s{new BooleanElement(false), new BooleanElement(false),
          new CommandElement("or")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("and true case", "[primitive][Boolean][and]") {
  Stack s{new BooleanElement(true), new BooleanElement(true),
          new CommandElement("and")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("and false case", "[primitive][Boolean][and]") {
  Stack s{new BooleanElement(true), new BooleanElement(false),
          new CommandElement("and")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("xor true case", "[primitive][Boolean][xor]") {
  Stack s{new BooleanElement(true), new BooleanElement(false),
          new CommandElement("xor")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("xor false case", "[primitive][Boolean][xor]") {
  Stack s{new BooleanElement(true), new BooleanElement(true),
          new CommandElement("xor")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}

TEST_CASE("eqv? true case", "[primitive][Boolean][eqv?]") {
  Stack s{new SubstackElement(Stack{new StringElement("first"),
                                    new StringElement("second"),
                                    new StringElement("third")}),
          new SubstackElement(Stack{new StringElement("first"),
                                    new StringElement("second"),
                                    new StringElement("third")}),
          new CommandElement("eqv?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == true);
}

TEST_CASE("eqv? false case", "[primitive][Boolean][eqv?]") {
  Stack s{new SubstackElement(Stack{new StringElement("first"),
                                    new StringElement("second"),
                                    new StringElement("third")}),
          new SubstackElement(Stack{new StringElement("first"),
                                    new StringElement("second"),
                                    new NumberElement(3, 0)}),
          new CommandElement("eqv?")};
  execute(s, emptyDefines);
  ElementPtr ptr(s.pop());
  REQUIRE(dynamic_cast<BooleanElement*>(ptr.get())->getData() == false);
}