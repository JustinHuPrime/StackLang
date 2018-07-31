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
#include "language/stackElements.h"

#include <assert.h>
#include <iostream>

#include "catch.hpp"

void testBoolean() noexcept;
void testCommand() noexcept;
void testNumber() noexcept;
void testString() noexcept;
void testSubstack() noexcept;
void testType() noexcept;

void testParsers() noexcept {
  using stacklang::StackElement;
  using stacklang::exceptions::ParserException;
  using stacklang::stackelements::NumberElement;
  using std::cerr;

  testBoolean();
  testNumber();
  testCommand();
  testString();
  testSubstack();
  testType();

  StackElement* elm;

  try {
    StackElement::parse("");
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ParserException& e) {
    assert(e.getContext() == "");
    assert(e.getLocation() == 0);
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }

  try {
    elm = StackElement::parse("22/7");
    NumberElement* num = dynamic_cast<NumberElement*>(elm);
    assert(num != nullptr);
    assert(num->getData() == mpq_class("22/7"));
  } catch (...) {
    cerr << " FAILED: Exception thrown, but expected none.\n";
    assert(false);
  }
}

void testBoolean() noexcept {
  using stacklang::exceptions::ParserException;
  using stacklang::stackelements::NumberElement;
  using std::cerr;
}

void testCommand() noexcept {}

void testNumber() noexcept {
  using stacklang::StackElement;
  using stacklang::exceptions::ParserException;
  using stacklang::stackelements::NumberElement;
  using std::cerr;

  StackElement* elm;

  try {
    NumberElement::parse("0x10c");
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ParserException& e) {
    assert(e.getContext() == "0x10c");
    assert(e.getLocation() == 1);
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
}

void testString() noexcept {}
void testSubstack() noexcept {}
void testType() noexcept {}