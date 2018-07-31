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
using stacklang::StackElement;
using stacklang::exceptions::ParserException;
using stacklang::stackelements::NumberElement;
}  // namespace

TEST_CASE("empty parse throws", "[parse][StackElement]") {
  REQUIRE_THROWS_AS(StackElement::parse(""), ParserException);
}

TEST_CASE("initial number parses as number", "[parse][StackElement]") {
  StackElement* elm = StackElement::parse("22/7");
  NumberElement* num = dynamic_cast<NumberElement*>(elm);
  REQUIRE(num != nullptr);
  REQUIRE(num->getData() == mpq_class("22/7"));
}

TEST_CASE("initial sign parses as number", "[parse][StackElement]") {
  StackElement* elm = StackElement::parse("-22/7");
  NumberElement* num = dynamic_cast<NumberElement*>(elm);
  REQUIRE(num != nullptr);
  REQUIRE(num->getData() == mpq_class("-22/7"));
}

TEST_CASE("initial inexact sign parses as number", "[parse][StackElement]") {
  StackElement* elm = StackElement::parse("~22/7");
  NumberElement* num = dynamic_cast<NumberElement*>(elm);
  REQUIRE(num != nullptr);
  REQUIRE(num->getData() == mpq_class("22/7"));
}

// void testNumber() noexcept {

//   try {
//     NumberElement::parse("0x10c");
//     cerr << " FAILED: Expected an exception, but none thrown.\n";
//     assert(false);
//   } catch (const ParserException& e) {
//     assert(e.getContext() == "0x10c");
//     assert(e.getLocation() == 1);
//   } catch (...) {
//     cerr << " FAILED: Threw wrong exception.\n";
//     assert(false);
//   }
// }