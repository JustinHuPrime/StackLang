// Copyright 2018 Justin Hu
//
// This file is part of the StackLang interpreter.
//
// The StackLang interpreter is free software: you can redistribute it and / or
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

// tests for the functions in stringUtils

#include "util/stringUtils.h"

#include <assert.h>
#include <string>

#include "catch.hpp"

TEST_CASE("Test starts with", "[stringUtils]") {
  using util::starts_with;
  CHECK(starts_with("abcd", "ab"));
  CHECK(!starts_with("abcd", "ad"));
  CHECK(starts_with("abcd", "abcd"));
}

TEST_CASE("Test ends_with", "[stringUtils]") {
  using util::ends_with;
  CHECK(ends_with("abcd", "cd"));
  CHECK(!ends_with("abcd", "ce"));
  CHECK(ends_with("abcd", "abcd"));
}

TEST_CASE("Test escape and unescape", "[stringUtils]") {
  using util::escape;
  using util::unescape;
  CHECK(escape("a\\b\"c\nd") == "a\\\\b\\\"c\\nd");
  CHECK(unescape("b\\\"a\\\\c\\nd") == "b\"a\\c\nd");
  CHECK(unescape(escape("a\\b\"c\n")) == "a\\b\"c\n");
  CHECK(escape(unescape("a\\\\c\\nb\\\"d")) == "a\\\\c\\nb\\\"d");
}

void testStringUtils() noexcept {
  using std::string;
  using util::ends_with;
  using util::escape;
  using util::findImproperEscape;
  using util::removeChar;
  using util::spaces;
  using util::starts_with;
  using util::trim;
  using util::unescape;
  assert(findImproperEscape("012345\\") == 6);
  assert(findImproperEscape("lotsa stuff \\\\") == string::npos);
  assert(findImproperEscape("0123456\\aother stuff goes here.") == 7);
  assert(findImproperEscape("012\\\\stuff after this...") == string::npos);
  assert(findImproperEscape("012\"stuff after this...") == 3);
  assert(findImproperEscape("012\\\\\"stuff after this...") == 5);
  assert(spaces(5) == "     ");
  assert(spaces(0) == "");
  assert(removeChar("aabaacdebba", 'a') == "bcdebb");
  assert(removeChar("doesn't contain that letter", 'j') ==
         "doesn't contain that letter");
  assert(trim(" \n\tabc \n\tdef\n \t") == "abc \n\tdef");
  assert(trim("no whitespace to trim") == "no whitespace to trim");
}