#include "ui/lineEditor.h"

#include <assert.h>
#include <string>

#include <iostream>

void testLineEditor() {
  using std::string;
  using TermUI::LineEditor;

  using std::cerr;

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