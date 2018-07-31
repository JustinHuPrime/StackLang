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

// Tests for the LineEditor class

#include "ui/lineEditor.h"

#include <string>

#include "catch.hpp"

namespace {
using std::string;
using terminalui::LineEditor;
}  // namespace

TEST_CASE("editor constructor sanity", "[lineEditor][constructor]") {
  LineEditor ed;
  REQUIRE(ed.isEmpty());
  REQUIRE(static_cast<string>(ed) == "");
}

TEST_CASE("editor adding strings", "[lineEditor][add]") {
  LineEditor ed;
  ed += "123456";
  REQUIRE(static_cast<string>(ed) == "123456");
  REQUIRE_FALSE(ed.isEmpty());
}

TEST_CASE("editor backspace", "[lineEditor][backspace]") {
  LineEditor ed;
  ed += "123456";
  ed.left();
  ed.backspace();
  REQUIRE(static_cast<string>(ed) == "12346");
  ed.toEnd();
  ed.backspace();
  REQUIRE(static_cast<string>(ed) == "1234");
}

TEST_CASE("editor add character", "[lineEditor][add]") {
  LineEditor ed;
  ed += "123456";
  ed.left();
  ed.backspace();
  ed += '5';
  REQUIRE(static_cast<string>(ed) == "123456");
}

TEST_CASE("editor delete", "[lineEditor][del]") {
  LineEditor ed;
  ed += "123456";
  ed.left();
  ed.del();
  REQUIRE(static_cast<string>(ed) == "12345");
  ed.toHome();
  ed.del();
  REQUIRE(static_cast<string>(ed) == "2345");
}

TEST_CASE("editor home and backspace limiting",
          "[lineEditor][toHome][backspace]") {
  LineEditor ed;
  ed += "12345";
  ed.toHome();
  ed.backspace();
  REQUIRE(static_cast<string>(ed) == "12345");
}

TEST_CASE("editor end and del limiting", "[lineEditor][toEnd][del]") {
  LineEditor ed;
  ed += "12345";
  ed.left();
  ed.toEnd();
  ed.del();
  REQUIRE(static_cast<string>(ed) == "12345");
}

TEST_CASE("editor clear", "[lineEditor][clear]") {
  LineEditor ed;
  ed += "123";
  ed.clear();
  REQUIRE(ed.isEmpty());
  REQUIRE(static_cast<string>(ed) == "");
}

TEST_CASE("empty editor backspace and del", "[lineEditor][del][backspace]") {
  LineEditor ed;
  ed.del();
  ed.backspace();
  REQUIRE(ed.cursorPosition() == 0);
}

TEST_CASE("editor initial cursor position",
          "[lineEditor][cursorPosition][constructor]") {
  LineEditor ed;
  REQUIRE(ed.cursorPosition() == 0);
}

TEST_CASE("editor moving cursor", "[lineEditor][cursorPosition]") {
  LineEditor ed;
  ed += "012345";
  ed.left();
  ed.left();
  REQUIRE(ed.cursorPosition() == 4);
}

TEST_CASE("editor initial history", "[lineEditor][constructor]") {
  LineEditor ed;
  ed += "something";
  ed.up();
  REQUIRE(static_cast<string>(ed) == "something");
  ed.down();
  REQUIRE(static_cast<string>(ed) == "something");
}

TEST_CASE("editor history scrolling", "[lineEditor][history]") {
  LineEditor ed;
  ed += "first";
  ed.enter();
  ed += "second";
  REQUIRE(static_cast<string>(ed) == "second");
  ed.enter();
  ed += "third";
  ed.enter();
  ed.up();
  ed.up();
  REQUIRE(static_cast<string>(ed) == "second");
  ed.down();
  REQUIRE(static_cast<string>(ed) == "third");
}

TEST_CASE("editor history immutability", "[lineEditor][history]") {
  LineEditor ed;
  ed += "first";
  ed.enter();
  ed += "second";
  ed.enter();
  ed += "third";
  ed.enter();
  ed.up();
  ed.clear();
  ed += "fourth";
  ed.enter();
  ed += "filler";
  ed.up();
  REQUIRE(static_cast<string>(ed) == "fourth");
  ed.up();
  REQUIRE(static_cast<string>(ed) == "third");
  ed.up();
  REQUIRE(static_cast<string>(ed) == "second");
  ed.up();
  REQUIRE(static_cast<string>(ed) == "first");
}

TEST_CASE("editor history grabbing", "[lineEditor][history]") {
  LineEditor ed;
  ed += "first";
  ed.enter();
  ed += "second";
  ed.enter();
  ed += "third";
  ed.enter();
  ed.up();
  ed.clear();
  ed += "fourth";
  ed.enter();
  ed.up();
  ed.up();
  ed.up();
  SECTION("at end of history") {
    ed.up();
    ed.enter();
    REQUIRE(ed.isEmpty());
    ed.up();
    REQUIRE(static_cast<string>(ed) == "first");
    ed.up();
    REQUIRE(static_cast<string>(ed) == "fourth");
    ed.up();
    REQUIRE(static_cast<string>(ed) == "third");
    ed.up();
    REQUIRE(static_cast<string>(ed) == "second");
    ed.up();
    REQUIRE(static_cast<string>(ed) == "first");
  }
  SECTION("in middle of history") {
    ed.enter();
    REQUIRE(ed.isEmpty());
    ed.up();
    REQUIRE(static_cast<string>(ed) == "second");
    ed.up();
    REQUIRE(static_cast<string>(ed) == "fourth");
    ed.up();
    REQUIRE(static_cast<string>(ed) == "third");
    ed.up();
    REQUIRE(static_cast<string>(ed) == "second");
    ed.up();
    REQUIRE(static_cast<string>(ed) == "first");
  }
}

TEST_CASE("editor limits on history and preserving current line",
          "[lineEditor][history]") {
  LineEditor ed;
  ed += "first";
  ed.enter();
  ed += "second";
  ed.enter();
  ed += "third";
  ed.up();
  ed.up();
  REQUIRE(static_cast<string>(ed) == "first");
  ed.up();
  REQUIRE(static_cast<string>(ed) == "first");
  ed.down();
  ed.down();
  REQUIRE(static_cast<string>(ed) == "third");
  ed.down();
  REQUIRE(static_cast<string>(ed) == "third");
}

TEST_CASE("editor clearing history", "[lineEditor][history]") {
  LineEditor ed;
  ed += "something";
  ed.enter();
  ed += "else";
  ed.enter();
  ed += "keep";
  ed.clearHist();
  REQUIRE(static_cast<string>(ed) == "keep");
  ed.up();
  REQUIRE(static_cast<string>(ed) == "keep");
  ed.down();
  REQUIRE(static_cast<string>(ed) == "keep");
}