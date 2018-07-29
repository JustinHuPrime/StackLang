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

#include <assert.h>
#include <string>

#include "catch.hpp"

void testLineEditor() noexcept {
  using std::string;
  using terminalui::LineEditor;

  LineEditor ed;
  assert(ed.isEmpty());
  assert(static_cast<string>(ed) == "");
  ed += "123456";
  assert(static_cast<string>(ed) == "123456");
  assert(!ed.isEmpty());
  ed.left();
  ed.backspace();
  assert(static_cast<string>(ed) == "12346");
  ed += '5';
  assert(static_cast<string>(ed) == "123456");
  ed.del();
  assert(static_cast<string>(ed) == "12345");
  ed.toHome();
  ed.backspace();
  assert(static_cast<string>(ed) == "12345");
  ed.toEnd();
  ed.del();
  assert(static_cast<string>(ed) == "12345");
  ed.right();
  ed.backspace();
  assert(static_cast<string>(ed) == "1234");
  ed.toHome();
  ed.del();
  assert(static_cast<string>(ed) == "234");
  assert(!ed.isEmpty());
  ed.clear();
  assert(static_cast<string>(ed) == "");
  assert(ed.isEmpty());
  ed += "012345";
  ed.left();
  ed.left();
  assert(ed.cursorPosition() == 4);
  ed.clear();
  ed.clearHist();
  ed += "first";
  ed.up();
  assert(static_cast<string>(ed) == "first");
  ed.down();
  assert(static_cast<string>(ed) == "first");
  ed.enter();
  ed += "second";
  assert(static_cast<string>(ed) == "second");
  ed.enter();
  ed += "third";
  ed.enter();
  ed.up();
  ed.up();
  assert(static_cast<string>(ed) == "second");
  ed.down();
  assert(static_cast<string>(ed) == "third");
  ed.clear();
  ed += "fourth";
  ed.enter();
  ed += "shouldn't be here.";
  ed.up();
  assert(static_cast<string>(ed) == "fourth");
  ed.up();
  assert(static_cast<string>(ed) == "third");
  ed.up();
  assert(static_cast<string>(ed) == "second");
  ed.up();
  assert(static_cast<string>(ed) == "first");
  ed.enter();
  assert(ed.isEmpty());
  ed.up();
  assert(static_cast<string>(ed) == "first");
  ed.up();
  assert(static_cast<string>(ed) == "fourth");
  ed.up();
  assert(static_cast<string>(ed) == "third");
  ed.up();
  assert(static_cast<string>(ed) == "second");
  ed.up();
  assert(static_cast<string>(ed) == "first");
  ed.down();
  ed.down();
  ed.enter();
  ed.up();
  assert(static_cast<string>(ed) == "third");
  ed.up();
  assert(static_cast<string>(ed) == "first");
  ed.up();
  assert(static_cast<string>(ed) == "fourth");
  ed.up();
  assert(static_cast<string>(ed) == "third");
  ed.up();
  assert(static_cast<string>(ed) == "second");
  ed.up();
  assert(static_cast<string>(ed) == "first");
  ed.clearHist();
  ed.up();
  assert(static_cast<string>(ed) == "first");
}