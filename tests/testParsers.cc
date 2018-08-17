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

// tests for the StackElement parse method

#include "language/exceptions/interpreterExceptions.h"
#include "language/stack/stackElements.h"

#include "catch.hpp"

namespace {
using stacklang::ElementPtr;
using stacklang::StackElement;
using stacklang::exceptions::ParserException;
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
}  // namespace

TEST_CASE("empty parse throws", "[parse][StackElement]") {
  REQUIRE_THROWS_AS(StackElement::parse(""), ParserException);
}

TEST_CASE("initial number parses as number", "[parse][StackElement][number]") {
  NumberPtr num(dynamic_cast<NumberElement*>(StackElement::parse("3.5")));
  REQUIRE(num != nullptr);
  REQUIRE(num->getData() == 3.5);
}

TEST_CASE("initial sign parses as number", "[parse][StackElement][number]") {
  NumberPtr num(dynamic_cast<NumberElement*>(StackElement::parse("-3.5")));
  REQUIRE(num != nullptr);
  REQUIRE(num->getData() == -3.5);
}

TEST_CASE("initial quote parses as string", "[parse][StackElement][string]") {
  StringPtr str(
      dynamic_cast<StringElement*>(StackElement::parse("\"string\"")));
  REQUIRE(str != nullptr);
  REQUIRE(str->getData() == "string");
}

TEST_CASE("substack delimiters recognized", "[parse][StackElement][substack]") {
  SubstackPtr s(dynamic_cast<SubstackElement*>(StackElement::parse("<< 5 >>")));
  REQUIRE(s != nullptr);
  REQUIRE(s->getType() == StackElement::DataType::Substack);
}

TEST_CASE("boolean true parses correctly", "[parse][StackElement][boolean]") {
  BooleanPtr b(dynamic_cast<BooleanElement*>(StackElement::parse("true")));
  REQUIRE(b != nullptr);
  REQUIRE(b->getData());
}

TEST_CASE("boolean false parses correctly", "[parse][StackElement][boolean]") {
  BooleanPtr b(dynamic_cast<BooleanElement*>(StackElement::parse("false")));
  REQUIRE(b != nullptr);
  REQUIRE_FALSE(b->getData());
}

TEST_CASE("parens parse to type", "[parse][StackElement][type]") {
  TypePtr t(
      dynamic_cast<TypeElement*>(StackElement::parse("Substack(Number)")));
  REQUIRE(t != nullptr);
  REQUIRE(t->getBase() == StackElement::DataType::Substack);
}

TEST_CASE("primitve type parses to type", "[parse][StackElement][type]") {
  TypePtr t(dynamic_cast<TypeElement*>(StackElement::parse("Number")));
  REQUIRE(t != nullptr);
  REQUIRE(t->getBase() == StackElement::DataType::Number);
}

TEST_CASE("word parses to command", "[parse][StackElement][command]") {
  CommandPtr cmd(dynamic_cast<CommandElement*>(StackElement::parse("cmd1*")));
  REQUIRE(cmd != nullptr);
  REQUIRE(cmd->getName() == "cmd1*");
}

TEST_CASE("command quote recognized", "[parse][StackElement][command]") {
  CommandPtr cmd(dynamic_cast<CommandElement*>(StackElement::parse("`Number")));
  REQUIRE(cmd != nullptr);
  REQUIRE(cmd->getName() == "Number");
}

TEST_CASE("unquoted command properly parsed", "[parse][CommandElement]") {
  CommandPtr cmd(CommandElement::parse("foldr"));
  REQUIRE(cmd->getName() == "foldr");
  REQUIRE_FALSE(cmd->isQuoted());
}

TEST_CASE("quoted command properly parsed", "[parse][CommandElement]") {
  CommandPtr cmd(CommandElement::parse("`foldr"));
  REQUIRE(cmd->getName() == "foldr");
  REQUIRE(cmd->isQuoted());
}

TEST_CASE("quote in middle of command", "[parse][CommandElement]") {
  REQUIRE_THROWS_AS(CommandElement::parse("`fold`r"), ParserException);
}

TEST_CASE("space in middle of command", "[parse][CommandElement]") {
  REQUIRE_THROWS_AS(CommandElement::parse("stuff todo"), ParserException);
}

TEST_CASE("symbol in middle of command", "[parse][CommandElement]") {
  REQUIRE_THROWS_AS(CommandElement::parse("things&others"), ParserException);
}

TEST_CASE("number with disallowed chars", "[parse][NumberElement]") {
  REQUIRE_THROWS_AS(NumberElement::parse("0x10c"), ParserException);
}

TEST_CASE("number with more than one symbol", "[parse][NumberElement]") {
  REQUIRE_THROWS_AS(NumberElement::parse("1..2"), ParserException);
}

TEST_CASE("number with symbol in the middle", "[parse][NumberElement]") {
  REQUIRE_THROWS_AS(NumberElement::parse("1+2"), ParserException);
  REQUIRE_THROWS_AS(NumberElement::parse("1-2"), ParserException);
}

TEST_CASE("number with quote chars between signs", "[parse][NumberElement]") {
  NumberPtr num(NumberElement::parse("+'1.5'"));
  REQUIRE(num->getData() == 1.5);
}

TEST_CASE("string with missing closing quote", "[parse][StringElement]") {
  REQUIRE_THROWS_AS(StringElement::parse("\"bad string"), ParserException);
}

TEST_CASE("string with bad escape", "[parse][StringElement]") {
  REQUIRE_THROWS_AS(StringElement::parse("\"bad\\string\""), ParserException);
}

TEST_CASE("valid string", "[parse][StringElement]") {
  StringPtr str(StringElement::parse("\"good\\nstring\""));
  StringPtr blank(StringElement::parse("\"\""));
  REQUIRE(str->getData() == "good\nstring");
  REQUIRE(blank->getData() == "");
}

TEST_CASE("substack with no closing delimiter", "[parse][SubstackElement]") {
  REQUIRE_THROWS_AS(SubstackElement::parse("<< 1, << 3 >>"), ParserException);
  REQUIRE_THROWS_AS(SubstackElement::parse("<< 1, << 3 >>,"), ParserException);
}

TEST_CASE("substack with extra closing delimiter", "[parse][SubstackElement]") {
  REQUIRE_THROWS_AS(SubstackElement::parse("<< 1 >>, >>"), ParserException);
}

TEST_CASE("regular substack parse", "[parse][SubstackElement]") {
  SubstackPtr s(SubstackElement::parse("<< 1, \">>\\\"string\\\"<<\">>"));
  auto iter = s->getData().begin();
  const NumberElement* num = dynamic_cast<const NumberElement*>(*iter);
  REQUIRE(num != nullptr);
  REQUIRE(num->getData() == 1);
  ++iter;
  const StringElement* str = dynamic_cast<const StringElement*>(*iter);
  REQUIRE(str != nullptr);
  REQUIRE(str->getData() == ">>\"string\"<<");
}

TEST_CASE("type base case good", "[parse][TypeElement]") {
  TypePtr t(TypeElement::parse("Number"));
  REQUIRE(t->getBase() == StackElement::DataType::Number);
  REQUIRE(t->getSpecialization() == nullptr);
}

TEST_CASE("type base case bad", "[parse][TypeElement]") {
  REQUIRE_THROWS_AS(TypeElement::parse("Junk"), ParserException);
}

TEST_CASE("substack type specialized good", "[parse][TypeElement]") {
  TypePtr t(TypeElement::parse("Substack(Any)"));
  REQUIRE(t->getBase() == StackElement::DataType::Substack);
  REQUIRE(t->getSpecialization()->getBase() == StackElement::DataType::Any);
  REQUIRE(t->getSpecialization()->getSpecialization() == nullptr);
}

TEST_CASE("substack type specialized bad", "[parse][TypeElement]") {
  REQUIRE_THROWS_AS(TypeElement::parse("Substack(Exact)"), ParserException);
}

TEST_CASE("unspecializable type specialzied", "[parse][TypeElement]") {
  REQUIRE_THROWS_AS(TypeElement::parse("String(Number)"), ParserException);
}