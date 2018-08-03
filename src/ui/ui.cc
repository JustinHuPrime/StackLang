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

// Implementation of interpreter-specific UI

#include "ui/ui.h"

#include <csignal>
#include <iostream>

#include <ncurses.h>

#include "language/language.h"
#include "util/stringUtils.h"

namespace terminalui {
namespace {
using stacklang::stopFlag;
using stacklang::stackelements::CommandElement;
using std::cerr;
using std::endl;
using util::spaces;
}  // namespace

void init() noexcept {
  setlocale(LC_ALL, "");

  initscr();

  cbreak();
  nonl();
  noecho();
  intrflush(stdscr, false);
  keypad(stdscr, true);

  atexit(uninit);

  signal(SIGWINCH, [](int sigNum) {
    (void)sigNum;  // ignore sigNum
    ungetch(EINTR);
  });

  signal(SIGINT, [](int sigNum) {
    (void)sigNum;  // ignore sigNum
    stopFlag = true;
  });

  auto defSigHandler = [](int sigNum) {
    uninit();
    signal(sigNum, SIG_DFL);
    raise(sigNum);
  };

  signal(SIGABRT, defSigHandler);
  signal(SIGFPE, defSigHandler);
  signal(SIGILL, defSigHandler);
  signal(SIGSEGV, defSigHandler);
  signal(SIGTERM, defSigHandler);
  signal(SIGQUIT, defSigHandler);
}

void uninit() noexcept { endwin(); }

void drawStack(const Stack& s) noexcept {
  int maxY = getmaxy(stdscr);
  int offset = getcurx(stdscr);

  int prevCurMode = curs_set(CURSOR_INVISIBLE);

  for (int i = 0; i < maxY - 1; i++) {
    move(i, 0);
    clrtoeol();
  }

  int i = maxY - 2;
  auto it = s.begin();
  for (; i > 0 && it != s.end(); i--, ++it) {
    move(i, 0);
    addstring(string(**it));
  }

  if (long(s.size()) >= getmaxy(stdscr) - 2) {
    move(0, 0);
    clrtoeol();
    addstring("...");
  }

  move(maxY - 1, 2 + offset);
  curs_set(prevCurMode);

  refresh();
}

void drawPrompt(const LineEditor& s) noexcept {
  int maxY = getmaxy(stdscr);

  curs_set(CURSOR_INVISIBLE);
  move(maxY - 1, 0);
  clrtoeol();
  addstring("> ");
  addstring(string(s));
  move(maxY - 1, 2 + s.cursorPosition());
  curs_set(CURSOR_VISIBLE);

  refresh();
}

void drawError(const LanguageException& e) noexcept {
  unsigned maxY = static_cast<unsigned>(getmaxy(stdscr));
  unsigned centerY = maxY / 2;

  curs_set(CURSOR_INVISIBLE);
  clear();

  if (e.getTrace().empty()) {  // no trace
    if (e.hasContext()) {
      move(centerY - 2, 0);
      addstring(e.getKind());

      move(centerY - 1, 0);
      addstring(e.getMessage());

      move(centerY + 0, 0);
      addstring(e.getContext());

      move(centerY + 1, 0);
      addstring(spaces(e.getLocation()) + '^');

      move(centerY + 3, 0);
      addstring("Press any key to continue...");
    } else {
      move(centerY - 1, 0);
      addstring(e.getKind());

      move(centerY + 0, 0);
      addstring(e.getMessage());

      move(centerY + 2, 0);
      addstring("Press any key to continue...");
    }
  } else {
    if (e.hasContext()) {
      move(0, 0);
      addstring(e.getKind());

      move(1, 0);
      addstring(e.getMessage());

      move(2, 0);
      addstring(e.getContext());

      move(3, 0);
      addstring(spaces(e.getLocation()) + '^');

      drawTrace(5, maxY - 3, e.getTrace());

      move(maxY - 1, 0);
      addstring("Press any key to continue...");
    } else {
      move(0, 0);
      addstring(e.getKind());

      move(1, 1);
      addstring(e.getMessage());

      drawTrace(3, maxY - 3, e.getTrace());

      move(maxY - 1, 0);
      addstring("Press any key to continue...");
    }
  }
}

void drawTrace(unsigned top, unsigned bottom,
               const list<CommandElement*>& trace) {
  if (bottom > top && trace.size() <= static_cast<size_t>(bottom - top)) {
    for (const CommandElement* elm : trace) {
      move(top++, 0);
      addstring("From " + (elm == nullptr ? "global context" : elm->getName()));
    }
  } else if (bottom > top) {
    unsigned dist = bottom - top + 1;
    unsigned topPart = dist / 2;
    unsigned bottomPart = dist / 2;
    if (dist == topPart + bottomPart) bottomPart--;
    auto iter = trace.begin();
    for (unsigned i = top; i < topPart + top; i++) {
      move(i, 0);
      addstring("From " +
                (*iter == nullptr
                     ? "global context"
                     : (**iter).getName()));  // shouldn't need this check.
      ++iter;
    }

    iter = trace.end();
    --iter;
    for (unsigned i = 3; i < bottomPart + 3; i++) {
      move(i, 0);
      addstring("From " +
                (*iter == nullptr ? "global context" : (**iter).getName()));
      --iter;
    }

    move(topPart, 0);
    addstring("...");
  }
}

void addstring(const string& s) noexcept {
  for (const char c : s) {
    unsigned char ch = static_cast<unsigned char>(c);
    addch(ch);
  }
}

void displayInfo() noexcept {
  curs_set(CURSOR_INVISIBLE);
  move(0, 0);
  addstring(INFO);
  getch();
}

void printError(const LanguageException& e) noexcept {
  cerr << e.getKind() << '\n';
  cerr << e.getMessage() << '\n';
  if (e.hasContext()) {
    cerr << e.getContext() << '\n';
    cerr << spaces(e.getLocation()) << "^" << '\n';
  }
  const list<CommandElement*>& stacktrace = e.getTrace();
  if (!stacktrace.empty()) {
    cerr << '\n';
    for (const CommandElement* ctx : stacktrace) {
      cerr << "In " << (ctx == nullptr ? "global context" : ctx->getName())
           << '\n';
    }
  }
  cerr.flush();
}
}  // namespace terminalui