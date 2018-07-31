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

// tests for the functions in stringUtils

#include "util/stringUtils.h"

#include <string>

#include "catch.hpp"

namespace {
using std::string;
using util::ends_with;
using util::escape;
using util::findImproperEscape;
using util::removeChar;
using util::spaces;
using util::starts_with;
using util::trim;
using util::unescape;
}  // namespace

TEST_CASE("starts with simple true case", "[stringUtils][starts_with]") {
  REQUIRE(starts_with("abcd", "ab"));
}

TEST_CASE("starts with simple false case", "[stringUtils][starts_with]") {
  REQUIRE_FALSE(starts_with("abcd", "ad"));
}

TEST_CASE("starts with true edge case", "[stringUtils][starts_with]") {
  REQUIRE(starts_with("abcd", "abcd"));
}

TEST_CASE("starts with complex false case", "[stringUtils][starts_with]") {
  REQUIRE_FALSE(starts_with("abcd", "abcde"));
}

TEST_CASE("ends_with simple true case", "[stringUtils][ends_with]") {
  REQUIRE(ends_with("abcd", "cd"));
}

TEST_CASE("ends_with simple false case", "[stringUtils][ends_with]") {
  REQUIRE_FALSE(ends_with("abcd", "ce"));
}

TEST_CASE("ends_with true edge case", "[stringUtils][ends_with]") {
  REQUIRE(ends_with("abcd", "abcd"));
}

TEST_CASE("ends_with complex false case", "[stringUtils][ends_with]") {
  REQUIRE_FALSE(ends_with("abcd", "0abcd"));
}

TEST_CASE("escape", "[stringUtils][escape]") {
  REQUIRE(escape("a\\b\"c\nd") == "a\\\\b\\\"c\\nd");
}

TEST_CASE("unescape", "[stringUtils][unescape]") {
  REQUIRE(unescape("b\\\"a\\\\c\\nd") == "b\"a\\c\nd");
}

TEST_CASE("escape/unescape identity", "[stringUtils][escape][unescape]") {
  REQUIRE(unescape(escape("a\\b\"c\n")) == "a\\b\"c\n");
  REQUIRE(escape(unescape("a\\\\c\\nb\\\"d")) == "a\\\\c\\nb\\\"d");
}

TEST_CASE("findImproperEscape escaped ending quote",
          "[stringUtils][findImproperEscape]") {
  REQUIRE(findImproperEscape("012345\\") == 6);
}

TEST_CASE("findImproperEscape nothing wrong ending quote",
          "[stringUtils][findImproperEscape]") {
  REQUIRE(findImproperEscape("lotsa stuff \\\\") == string::npos);
}

TEST_CASE("findImproperEscape bad escape",
          "[stringUtils][findImproperEscape]") {
  REQUIRE(findImproperEscape("0123456\\aother stuff goes here.") == 7);
}

TEST_CASE("findImproperEscape nothing wrong escaped escape",
          "[stringUtils][findImproperEscape]") {
  REQUIRE(findImproperEscape("012\\\\stuff after this...") == string::npos);
}

TEST_CASE("findImproperEscape unescaped quote",
          "[stringUtils][findImproperEscape]") {
  REQUIRE(findImproperEscape("012\"stuff after this...") == 3);
}

TEST_CASE("findImproperEscape backslash before unescaped quote",
          "[stringUtils][findImproperEscape]") {
  REQUIRE(findImproperEscape("012\\\\\"stuff after this...") == 5);
}

TEST_CASE("spaces function simple", "[stringUtils][spaces]") {
  REQUIRE(spaces(5) == "     ");
}

TEST_CASE("spaces function edge case", "[stringUtils][spaces]") {
  REQUIRE(spaces(0) == "");
}

TEST_CASE("removeChar simple", "[stringUtils][removeChar]") {
  REQUIRE(removeChar("aabaacdebba", 'a') == "bcdebb");
}

TEST_CASE("removeChar no change", "[stringUtils][removeChar]") {
  REQUIRE(removeChar("doesn't contain that letter", 'j') ==
          "doesn't contain that letter");
}

TEST_CASE("trim with whitespace", "[stringUtils][trim]") {
  REQUIRE(trim(" \n\tabc \n\tdef\n \t") == "abc \n\tdef");
}

TEST_CASE("trim no whitespace", "[stringUtils][trim]") {
  REQUIRE(trim("no whitespace to trim") == "no whitespace to trim");
}