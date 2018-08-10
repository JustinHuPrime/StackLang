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

// Main function for the interpreter. Parses command line arugments then runs
// main loop of ui.

#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include <ncurses.h>

#include "language/exceptions/languageExceptions.h"
#include "language/language.h"
#include "language/stack/stack.h"
#include "language/stack/stackElements.h"
#include "ui/argReader.h"
#include "ui/lineEditor.h"
#include "ui/ui.h"

namespace {
using stacklang::DefinedFunction;
using stacklang::Stack;
using stacklang::StackElement;
using stacklang::stopFlag;
using stacklang::exceptions::LanguageException;
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::StringElement;
using std::cerr;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::numeric_limits;
using std::stoi;
using std::stoul;
using std::string;
using std::vector;
using terminalui::addstring;
using terminalui::ArgReader;
using terminalui::displayInfo;
using terminalui::drawError;
using terminalui::drawPrompt;
using terminalui::drawStack;
using terminalui::drawWaiting;
using terminalui::HELPMSG;
using terminalui::init;
using terminalui::LineEditor;
using terminalui::printError;

const char KEY_CTRL_D = 4;  // FIXME: ctrl-d and EINTR collide.
}  // namespace

int main(int argc, char* argv[]) noexcept {
  Stack s;
  Defines defines;

  LineEditor buffer;
  bool errorFlag = false;

  int debugMode = 0;
  string outputFile;

  ArgReader args;

  // flags parsing

  try {
    args.read(argc, argv);
    args.validate("?hb", "dlo", "I");
  } catch (const LanguageException& e) {
    printError(e);
    cerr << "Encountered error parsing command line arguments. Aborting."
         << endl;
    exit(EXIT_FAILURE);
  }
  if (args.hasFlag('?') || args.hasFlag('h')) {
    cout << HELPMSG;
    exit(EXIT_SUCCESS);
  }
  if (!args.hasFlag('b')) {
    s.push(new StringElement("std"));
    s.push(new CommandElement("include"));
    try {
      stopFlag = false;
      execute(s, defines);
    } catch (const LanguageException& e) {
      printError(e);
      cerr << "Encountered error in standard library. Please report this "
              "error. Aborting."
           << endl;
      exit(EXIT_FAILURE);
    }
  }
  if (args.hasOpt('d')) {
    try {
      debugMode = stoi(args.getOpt('d'));
    } catch (const invalid_argument&) {
      cerr << "(Command line arguments invalid:\nExpected a number after "
              "`-d`, "
              "but found" +
                  args.getOpt('d') + ".\nAborting."
           << endl;
      exit(EXIT_FAILURE);
    }
  }
  if (args.hasOpt('l')) {
    try {
      s.setLimit(stoul(args.getOpt('l')));
    } catch (const invalid_argument&) {
      cerr << "(Command line arguments invalid:\nExpected a number after `-l`, "
              "but found" +
                  args.getOpt('l') + ".\nAborting."
           << endl;
      exit(EXIT_FAILURE);
    } catch (const LanguageException& e) {
      printError(e);
      cerr << "Encountered error parsing command line arguments. Aborting."
           << endl;
      exit(EXIT_FAILURE);
    }
  }
  if (args.hasOpt('o')) {
    outputFile = args.getOpt('o');
  }
  if (args.hasOpt('I')) {
    vector<string> libs = args.getLongOpt('I');
    for (string str : libs) {
      s.push(new StringElement(str));
      s.push(new CommandElement("include"));
      try {
        stopFlag = false;
        execute(s, defines);
      } catch (const LanguageException& e) {
        printError(e);
        cerr << "Encountered error parsing command line arguments. Aborting."
             << endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  // TUI stuff

  init();

  displayInfo();  // splash screen

  drawStack(s);
  drawPrompt(buffer);

  while (true) {
    int key = getch();

    if (key == KEY_CTRL_D) {  // overriding keypresses
      break;
    } else if (key == EINTR) {
      endwin();  // these commands resync ncurses with the terminal
      refresh();

      clear();
      drawStack(s);
      drawPrompt(buffer);
    } else if (errorFlag) {  // anything on an error is ignored, but the error
                             // is cleared
      drawStack(s);
      drawPrompt(buffer);
      errorFlag = false;
      continue;
    }

    if (key < numeric_limits<char>().max() && isprint(key) && key != '\n' &&
        key != '\r' &&
        key != KEY_ENTER) {  // normal characters added to buffer.
      buffer += key;
      drawPrompt(buffer);
    } else if ((key == '\n' || key == '\r' || key == KEY_ENTER) &&
               !buffer.isEmpty()) {  // enter - add and execute
      string bufferStr = string(buffer);
      buffer.enter();
      try {
        s.push(StackElement::parse(bufferStr));
        drawStack(s);
        drawWaiting();
        stopFlag = false;
        execute(s, defines);
        drawStack(s);
        drawPrompt(buffer);
      } catch (const LanguageException& e) {
        drawError(e);
        errorFlag = true;
      }
    } else if (key == KEY_BACKSPACE) {  // line editing.
      buffer.backspace();
      drawPrompt(buffer);
    } else if (key == KEY_DC) {
      buffer.del();
      drawPrompt(buffer);
    } else if (key == KEY_UP) {
      buffer.up();
      drawPrompt(buffer);
    } else if (key == KEY_DOWN) {
      buffer.down();
      drawPrompt(buffer);
    } else if (key == KEY_RIGHT) {
      buffer.right();
      drawPrompt(buffer);
    } else if (key == KEY_LEFT) {
      buffer.left();
      drawPrompt(buffer);
    } else if (key == KEY_END) {
      buffer.toEnd();
      drawPrompt(buffer);
    } else if (key == KEY_HOME) {
      buffer.toHome();
      drawPrompt(buffer);
    } else {  // not recognized.
      beep();
    }

    if (debugMode == 1) {  // debug options - must not include 0
      move(0, getmaxx(stdscr) - 3);
      clrtoeol();
      addstring(std::to_string(key).c_str());
      move(getmaxy(stdscr) - 1, buffer.cursorPosition() + 2);
    } else if (debugMode == 2) {
      move(0, 0);
      clrtoeol();
      addstring("|");
      addstring(string(buffer));
      addstring("|");
      move(getmaxy(stdscr) - 1, buffer.cursorPosition() + 2);
    }
  }

  exit(EXIT_SUCCESS);
}