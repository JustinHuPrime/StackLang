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

// tests for the Stack class

#include "language/exceptions/languageExceptions.h"
#include "language/stack.h"
#include "language/stackElements.h"

#include <limits>

#include "catch.hpp"

namespace {
using stacklang::Stack;
using stacklang::StackElement;
using stacklang::exceptions::StackOverflowError;
using stacklang::exceptions::StackUnderflowError;
using stacklang::stackelements::NumberElement;
using std::numeric_limits;
}  // namespace

TEST_CASE("stack constructor sanity", "[stack][constructor]") {
  Stack s;
  REQUIRE(s.isEmpty());
}

TEST_CASE("stack initializer", "[stack][constructor]") {
  NumberElement* elm1 = new NumberElement("1");
  NumberElement* elm2 = new NumberElement("2");
  Stack s{elm1, elm2};
  REQUIRE(s.top() == elm2);
  delete s.pop();
  REQUIRE(s.top() == elm1);
  delete s.pop();
  REQUIRE(s.isEmpty());
}

TEST_CASE("stack push, top, and pop", "[stack][push][pop][top]") {
  Stack s;
  NumberElement* elm1 = new NumberElement("1");
  NumberElement* elm2 = new NumberElement("2");
  s.push(elm1);
  REQUIRE_FALSE(s.isEmpty());
  REQUIRE(s.top() == elm1);
  s.push(elm2);
  StackElement* disc = s.pop();
  REQUIRE(disc == elm2);
  REQUIRE(s.top() == elm1);
  delete disc;
}

TEST_CASE("stack clearing", "[stack][clear]") {
  NumberElement* elm1 = new NumberElement("1");
  NumberElement* elm2 = new NumberElement("2");
  Stack s{elm1, elm2};
  s.clear();
  REQUIRE(s.isEmpty());
}

TEST_CASE("stack reverse", "[stack][reverse]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  StackElement* elm3 = new NumberElement("3");
  Stack s{elm1, elm2, elm3};
  s.reverse();
  REQUIRE(s.pop() == elm1);
  REQUIRE(s.pop() == elm2);
  REQUIRE(s.pop() == elm3);
  REQUIRE(s.isEmpty());
}

TEST_CASE("default stack limit", "[stack][getLimit][constructor]") {
  Stack s;
  REQUIRE(s.getLimit() == numeric_limits<unsigned long>().max());
}

TEST_CASE("stack size", "[stack][size]") {
  Stack s;
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  REQUIRE(s.size() == 0);
  s.push(elm1);
  s.push(elm2);
  REQUIRE(s.size() == 2);
  s.clear();
  REQUIRE(s.size() == 0);
}

TEST_CASE("stack copy constructor", "[stack][constructor]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  Stack s{elm1, elm2};
  Stack sprime = s;
  REQUIRE(sprime.top() != elm2);
  REQUIRE(dynamic_cast<const NumberElement*>(sprime.top())->getData() == 2);
  delete sprime.pop();
  REQUIRE(sprime.top() != elm1);
  REQUIRE(dynamic_cast<const NumberElement*>(sprime.top())->getData() == 1);
  delete sprime.pop();
  REQUIRE(sprime.isEmpty());
}

TEST_CASE("stack move constructor", "[stack][constructor]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  Stack sprime = [&elm1, &elm2]() {
    Stack s{elm1, elm2};
    return s;
  }();
  REQUIRE(sprime.top() == elm2);
  delete sprime.pop();
  REQUIRE(sprime.top() == elm1);
  delete sprime.pop();
  REQUIRE(sprime.isEmpty());
}

TEST_CASE("stack iterator begin, end", "[stack][stackIterator][begin][end]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  Stack s{elm1, elm2};
  REQUIRE(*s.begin() == elm2);
  auto iter = s.begin();
  ++iter;
  ++iter;
  REQUIRE(iter == s.end());
}

TEST_CASE("stack iterator increment", "[stack][stackIterator][increment]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  Stack s{elm1, elm2};
  auto iter = s.begin();
  iter++;
  REQUIRE(*iter == elm1);
  ++iter;
  REQUIRE(iter == s.end());
}

TEST_CASE("stack setLimit valid", "[stack][setLimit]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  Stack s{elm1, elm2};
  REQUIRE_NOTHROW(s.setLimit(3));
  REQUIRE(s.getLimit() == 3);
}

TEST_CASE("stack setLimit too small", "[stack][setLimit]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  Stack s{elm1, elm2};
  REQUIRE_THROWS_AS(s.setLimit(1), StackOverflowError);
}

TEST_CASE("stack setLimit edge case", "[stack][setLimit]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  Stack s{elm1, elm2};
  REQUIRE_NOTHROW(s.setLimit(2));
  REQUIRE(s.getLimit() == 2);
}

TEST_CASE("stack pop underflow error", "[stack][pop]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  Stack s{elm1, elm2};
  REQUIRE_NOTHROW(s.pop());
  delete elm2;
  REQUIRE_NOTHROW(s.pop());
  delete elm1;
  REQUIRE_THROWS_AS(s.pop(), StackUnderflowError);
}

TEST_CASE("stack top underflow error", "[stack][top]") {
  StackElement* elm1 = new NumberElement("1");
  Stack s{elm1};
  REQUIRE_NOTHROW(s.top());
  delete s.pop();
  REQUIRE_THROWS_AS(s.top(), StackUnderflowError);
}

TEST_CASE("stack push overflow error", "[stack][push]") {
  StackElement* elm1 = new NumberElement("1");
  StackElement* elm2 = new NumberElement("2");
  StackElement* elm3 = new NumberElement("3");
  Stack s(2);
  REQUIRE_NOTHROW(s.push(elm1));
  REQUIRE_NOTHROW(s.push(elm2));
  REQUIRE_THROWS_AS(s.push(elm3), StackOverflowError);
}

TEST_CASE("stack constructor with size limit sanity", "[stack][constructor]") {
  Stack s(4);
  REQUIRE(s.getLimit() == 4);
  REQUIRE(s.isEmpty());
}