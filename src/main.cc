// Copyright 2018 Justin Hu, Bronwyn Damm, Jacques Marais, Ramon Rakow, and Jude
// Sidloski
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

#include <ncurses.h>

#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "language/environment.h"
#include "language/exceptions/languageExceptions.h"
#include "language/language.h"
#include "language/stack/stack.h"
#include "language/stack/stackElements.h"
#include "ui/argReader.h"
#include "ui/lineEditor.h"
#include "ui/ui.h"

namespace {
using stacklang::EnvTree;
using stacklang::Stack;
using stacklang::StackElement;
using stacklang::stopFlag;
using stacklang::exceptions::LanguageException;
using stacklang::stackelements::IdentifierElement;
using stacklang::stackelements::StringElement;
using std::cerr;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::numeric_limits;
using std::ofstream;
using std::stoi;
using std::stoul;
using std::string;
using std::vector;
using terminalui::addString;
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
using terminalui::uninit;

const char KEY_CTRL = 0x1f;
const char KEY_CTRL_D = 'd' & KEY_CTRL;
const char KEY_CTRL_X = 'x' & KEY_CTRL;

void outputToFile(ofstream& outputFile, Stack& s) {
  if (outputFile.is_open()) {
    s.reverse();
    for (auto elm : s) outputFile << static_cast<string>(*elm) << '\n';
    outputFile.close();
  }
}
}  // namespace

int main(int argc, char* argv[]) noexcept {
  Stack s;
  EnvTree e;

  LineEditor buffer;
  bool errorFlag = false;

  int debugMode = 0;  // currently has no effect
  ofstream outputFile;

  ArgReader args;

  // flags parsing
  try {
    args.read(argc, const_cast<const char**>(argv));
    args.validate("?bh", "dfloI", "I");
  } catch (const LanguageException& exn) {
    printError(exn);
    cerr << "\nEncountered error parsing command line arguments. Aborting."
         << endl;
    exit(EXIT_FAILURE);
  }

  if (args.hasFlag('?') ||
      args.hasFlag('h')) {  // out of order - processed first since help options
                            // supercede other options
    cout << HELPMSG;
    exit(EXIT_SUCCESS);
  }

  if (!args.hasFlag('b')) {
    s.push(new StringElement("std"));
    s.push(new IdentifierElement("include"));
    try {
      stopFlag = false;
      execute(s, e.getRoot());
    } catch (const LanguageException& exn) {
      printError(exn);
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
              "`-d`, but found" +
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
    } catch (const LanguageException& exn) {
      printError(exn);
      cerr << "Encountered error parsing command line arguments. Aborting."
           << endl;
      exit(EXIT_FAILURE);
    }
  }
  if (args.hasOpt('o')) {
    outputFile.open(args.getOpt('o'), ofstream::trunc | ofstream::out);
    if (!outputFile.is_open()) {
      cerr << "Could not open output file.\nAborting." << endl;
      exit(EXIT_FAILURE);
    }
  }
  if (args.hasLongOpt('I') || args.hasOpt('I')) {
    vector<string> libs = args.hasOpt('I') ? vector<string>{args.getOpt('I')}
                                           : args.getLongOpt('I');
    for (string str : libs) {
      s.push(new StringElement(str));
      s.push(new IdentifierElement("include"));

      try {
        stopFlag = false;
        execute(s, e.getRoot());
      } catch (const LanguageException& exn) {
        printError(exn);
        cerr << "Encountered error parsing command line arguments. Aborting."
             << endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  if (args.hasOpt('f')) {  // out of order - must be after other includes have
                           // been processed.
    s.push(new StringElement(args.getOpt('f')));
    s.push(new IdentifierElement("include"));

    try {
      stopFlag = false;
      execute(s, e.getRoot());
    } catch (const LanguageException& exn) {
      printError(exn);
      cerr << "Encountered error running interpreted file. Aborting." << endl;
      exit(EXIT_FAILURE);
    }

    if (outputFile.is_open()) outputToFile(outputFile, s);

    exit(EXIT_SUCCESS);
  }

  // TUI stuff
  init();

  displayInfo();  // splash screen

  drawStack(s);
  drawPrompt(buffer);

  // Main loop
  while (true) {
    int key = getch();

    if (key == ERR) {
      continue;                      // do nothing on no input.
    } else if (key == KEY_CTRL_D) {  // overriding keypresses
      break;
    } else if (key == KEY_CTRL_X) {
      if (!s.isEmpty()) {
        delete s.pop();
        drawStack(s);
        drawPrompt(buffer);
      }
      continue;
    } else if (key == KEY_RESIZE) {
      endwin();  // these commands resync ncurses with the terminal
      refresh();

      clear();
      drawStack(s);
      drawPrompt(buffer);
      continue;
    } else if (errorFlag && key != ERR) {  // anything on an error is ignored,
                                           // but the error is cleared
      drawStack(s);
      drawPrompt(buffer);
      errorFlag = false;
      continue;
    }

    if (isprint(key)) {  // normal characters added to buffer.
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
        execute(s, e.getRoot());
        drawStack(s);
        drawPrompt(buffer);
      } catch (const LanguageException& exn) {
        drawError(exn);
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

    // if (debugMode == 1) {  // debug options - must not include 0
    //   int curx = getcurx(stdscr);
    //   move(0, 0);
    //   clrtoeol();
    //   addstring("|");
    //   addstring(to_string(curx) + "/" + to_string(getmaxx(stdscr)));
    //   addstring("|");
    //   move(getmaxy(stdscr) - 1, buffer.cursorPosition() + 2);
    // } else if (debugMode == 2) {
    //   move(0, 0);
    //   clrtoeol();
    //   addstring("|");
    //   addstring(string(buffer));
    //   addstring("|");
    //   move(getmaxy(stdscr) - 1, buffer.cursorPosition() + 2);
    // }
  }

  uninit();

  outputToFile(outputFile, s);

  exit(EXIT_SUCCESS);
}
