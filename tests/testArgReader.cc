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

// Tests for the ArgReader class

#include "language/exceptions/interpreterExceptions.h"
#include "ui/argReader.h"

#include <string>
#include <vector>

#include "catch.hpp"

namespace {
using Catch::Matchers::Equals;
using stacklang::exceptions::ArgumentError;
using std::string;
using std::vector;
using terminalui::ArgReader;
char* argv[] = {"discard", "-a",    "-b",       "data",  "-c",        "more",
                "stuff",   "-d",    "\"-words", "in",    "quotes-\"", "-e",
                "\"part",  "one\"", "\"part",   "two\"", "three"};
}  // namespace

TEST_CASE("argReader discards bad argv", "[argReader][read]") {
  ArgReader ar;
  char* badArgv[] = {"discard", "-ab", "-f"};
  REQUIRE_THROWS_AS(ar.read(3, badArgv), ArgumentError);
}

TEST_CASE("argReader read and validate args", "[argReader][read][validate]") {
  ArgReader ar;
  REQUIRE_NOTHROW(ar.read(17, argv));
  REQUIRE_NOTHROW(ar.validate("a", "bd", "ce"));
}

TEST_CASE("argReader validate", "[argReader][validate]") {
  ArgReader ar;
  ar.read(17, argv);
  REQUIRE_THROWS_AS(ar.validate("", "bd", "ce"), ArgumentError);
  REQUIRE_THROWS_AS(ar.validate("a", "d", "ce"), ArgumentError);
  REQUIRE_THROWS_AS(ar.validate("a", "bd", "c"), ArgumentError);
}

TEST_CASE("argReader has* predicates",
          "[argReader][hasFlag][hasOpt][hasLongOpt]") {
  ArgReader ar;
  ar.read(17, argv);
  REQUIRE(ar.hasFlag('a'));
  REQUIRE_FALSE(ar.hasFlag('b'));
  REQUIRE_FALSE(ar.hasFlag('x'));

  REQUIRE(ar.hasOpt('b'));
  REQUIRE_FALSE(ar.hasOpt('c'));
  REQUIRE_FALSE(ar.hasOpt('x'));

  REQUIRE(ar.hasLongOpt('c'));
  REQUIRE_FALSE(ar.hasLongOpt('a'));
  REQUIRE_FALSE(ar.hasLongOpt('x'));
}

TEST_CASE("argReader get*", "[argReader][getOpt][getLongOpt]") {
  ArgReader ar;
  ar.read(17, argv);
  REQUIRE(ar.getOpt('b') == "data");
  REQUIRE(ar.getOpt('d') == "-words in quotes-");
  REQUIRE_THROWS_AS(ar.getOpt('c'), ArgumentError);
  REQUIRE_THROWS_AS(ar.getOpt('x'), ArgumentError);
  REQUIRE_THAT(ar.getLongOpt('c'), Equals(vector<string>{"more", "stuff"}));
  REQUIRE_THAT(ar.getLongOpt('e'),
               Equals(vector<string>{"part one", "part two", "three"}));
  REQUIRE_THROWS_AS(ar.getLongOpt('b'), ArgumentError);
  REQUIRE_THROWS_AS(ar.getLongOpt('z'), ArgumentError);
}