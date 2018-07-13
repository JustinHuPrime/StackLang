#include "ui/ui.h"

#include <csignal>
#include <iostream>

#include <ncurses.h>

#include "util/stringUtils.h"

namespace TermUI {
using std::cerr;
using std::endl;
using Util::spaces;

void init() {
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

  auto defSigHandler = [](int sigNum) {
    uninit();
    signal(sigNum, SIG_DFL);
    raise(sigNum);
  };

  signal(SIGINT, defSigHandler);

  signal(SIGABRT, defSigHandler);
  signal(SIGFPE, defSigHandler);
  signal(SIGILL, defSigHandler);
  signal(SIGSEGV, defSigHandler);
  signal(SIGTERM, defSigHandler);
  signal(SIGQUIT, defSigHandler);
}

void uninit() { endwin(); }

void drawStack(const Stack& s) {
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

void drawPrompt(const LineEditor& s) {
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

void drawError(const LanguageException& e) {
  int centerY = getmaxy(stdscr) / 2;

  curs_set(CURSOR_INVISIBLE);
  clear();

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
}

void addstring(const string& s) {
  for (const char c : s) {
    unsigned char ch = static_cast<unsigned char>(c);
    addch(ch);
  }
}

void displayInfo() {
  curs_set(CURSOR_INVISIBLE);
  move(0, 0);
  addstring(INFO);
  getch();
}

void printError(const LanguageException& e) {
  cerr << e.getKind() << endl;
  cerr << e.getMessage() << endl;
  if (e.hasContext()) {
    cerr << e.getContext() << endl;
    cerr << spaces(e.getLocation()) << "^" << endl;
  }
}
}  // namespace TermUI