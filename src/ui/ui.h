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

// Interpreter specific UI. Some parts are very generic (init, uninit, draw*,
// addString, displayInfo)

#ifndef STACKLANG_UI_UI_H_
#define STACKLANG_UI_UI_H_

#include <string>

#include "language/exceptions/languageExceptions.h"
#include "language/stack.h"
#include "language/stackElements.h"
#include "ui/lineEditor.h"

namespace terminalui {
namespace {
using stacklang::Stack;
using stacklang::StackElement;
using stacklang::exceptions::LanguageException;
using std::string;
using terminalui::LineEditor;
}  // namespace

// initializes/uninitializes curses (and general stuff) with preset options
void init() noexcept;
void uninit() noexcept;

void drawStack(const Stack&) noexcept;
void drawPrompt(const LineEditor&) noexcept;
void drawError(const LanguageException&) noexcept;

// adds an std::string at the current cursor position
void addstring(const string& s) noexcept;

// displays info splash, then waits for a key
void displayInfo() noexcept;

// prints an error mesage to stderr
void printError(const LanguageException&) noexcept;

const int CURSOR_INVISIBLE = 0;
const int CURSOR_VISIBLE = 1;
const int CURSOR_VERY_VISIBLE = 2;

const char INFO[] = R"(StackLang interpreter version ALPHA 6
Copyright 2018 Justin Hu
This program comes with ABSOLUTELY NO WARRANTY. This is free software, and you
are welcome to redistribute it under certain conditions. For more detail see
the COPYING file that should have been included with this program.

Use ^D to exit from prompt, and ^C to force quit.
Press any key to continue...)";

const char HELPMSG[] = R"(Usage: stacklang [OPTIONS]
* `-?`, `-h`: prints this message.
* `-W`: show warranty information.
* `-C`: show copying information
* `-d N`: sets debugger to mode N.
* `-I filepath ... --`: includes files at filepath.
* `-l N`: limits stack to N elements in size.
* `-o file`: outputs formatted stack to file.
)";
}  // namespace terminalui

#endif  // STACKLANG_UI_UI_H_