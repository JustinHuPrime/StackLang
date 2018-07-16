#ifndef STACKLANG_UI_UI_H_
#define STACKLANG_UI_UI_H_

#include <string>

#include "language/exceptions/languageException.h"
#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackIterator.h"
#include "ui/lineEditor.h"

namespace TermUI {
using StackLang::Stack;
using StackLang::StackElement;
using StackLang::Exceptions::LanguageException;
using std::string;
using Util::LineEditor;

// initializes/uninitializes curses (and general stuff) with preset options
void init();
void uninit();

void drawStack(const Stack&);
void drawPrompt(const LineEditor&);
void drawError(const LanguageException&);

// adds an std::string at the current cursor position
void addstring(const string& s);

// displays info splash, then waits for a key
void displayInfo();

// prints an error mesage to stderr
void printError(const LanguageException&);

const int CURSOR_INVISIBLE = 0;
const int CURSOR_VISIBLE = 1;
const int CURSOR_VERY_VISIBLE = 2;

const char* const INFO = R"(StackLang interpreter version ALPHA 5
by Justin Hu, 2018
This is free software with ABSOLUTELY NO WARRANTY.
See the COPYING.md file for information on redistribution.
Use ^D to exit from prompt, and ^C to force quit
Press any key to continue...)";

const char* const HELPMSG = R"( Usage: stacklang [OPTIONS]
* `-?`, `-h`: prints this message.
* `-d N`: sets debugger to mode N.
* `-I filepath ... --`: includes files at filepath.
* `-l N`: limits stack to N elements in size.
* `-o file`: outputs formatted stack to file.
)";
}  // namespace TermUI

#endif  // STACKLANG_UI_UI_H_