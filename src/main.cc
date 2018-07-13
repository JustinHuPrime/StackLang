#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <ncurses.h>

#include "language/exceptions/languageException.h"
#include "language/language.h"
#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/stringElement.h"
#include "ui/argReader.h"
#include "ui/lineEditor.h"
#include "ui/ui.h"

namespace KeyInfo {
const char KEY_CTRL_D = 4;
}

int main(int argc, char* argv[]) {
  using StackLang::DefineMap;
  using StackLang::Stack;
  using StackLang::StackElement;
  using StackLang::Exceptions::LanguageException;
  using StackLang::StackElements::CommandElement;
  using StackLang::StackElements::StringElement;
  using std::cerr;
  using std::cout;
  using std::endl;
  using std::invalid_argument;
  using std::map;
  using std::numeric_limits;
  using std::stoi;
  using std::stoul;
  using std::string;
  using std::vector;
  using TermUI::addstring;
  using TermUI::ArgReader;
  using TermUI::displayInfo;
  using TermUI::drawError;
  using TermUI::drawPrompt;
  using TermUI::drawStack;
  using TermUI::HELPMSG;
  using TermUI::init;
  using TermUI::LineEditor;
  using TermUI::printError;

  Stack s;
  DefineMap defines;

  LineEditor buffer;
  bool errorFlag = false;

  int debugMode = 0;
  string outputFile;

  ArgReader args;

  try {
    args.read(argc, argv);
    args.validate("?h", "dlo", "I");
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
  if (args.hasOpt('d')) {
    try {
      debugMode = stoi(args.getOpt('d'));
    } catch (const invalid_argument&) {
      cerr << "(Command line arguments invalid:\nExpected a number after `-d`, "
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
        execute(s, defines);
      } catch (const LanguageException& e) {
        printError(e);
        cerr << "Encountered error parsing command line arguments. Aborting."
             << endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  init();

  displayInfo();  // splash screen

  drawStack(s);
  drawPrompt(buffer);

  while (true) {
    int key = getch();

    if (key == KeyInfo::KEY_CTRL_D) {  // overriding keypresses
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
    } else if (key == '\n' || key == '\r' ||
               key == KEY_ENTER) {  // enter - add and execute
      string bufferStr = string(buffer);
      buffer.enter();
      try {
        s.push(StackElement::parse(bufferStr));
        drawStack(s);
        drawPrompt(buffer);
        execute(s, defines);
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