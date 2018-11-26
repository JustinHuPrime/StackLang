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

// Interpreter specific UI. Some parts are very generic (init, uninit, draw*,
// addString, displayInfo)

#ifndef STACKLANG_UI_UI_H_
#define STACKLANG_UI_UI_H_

#include <string>
#include <vector>

#include "language/exceptions/languageExceptions.h"
#include "language/stack/stack.h"
#include "language/stack/stackElements.h"
#include "ui/lineEditor.h"

namespace terminalui {
// initializes/uninitializes curses (and general stuff) with preset options
void init() noexcept;
void uninit() noexcept;

void drawStack(const stacklang::Stack&) noexcept;
void drawPrompt(const LineEditor&) noexcept;
void drawWaiting() noexcept;
void drawError(const stacklang::exceptions::LanguageException&) noexcept;
void drawTrace(int, int, const std::vector<std::string>&);

// adds an string at the current cursor position
void addString(const std::string& s) noexcept;
void addBlock(const std::string& s) noexcept;

// displays info splash, then waits for a key
void displayInfo() noexcept;

// prints an error mesage to stderr
void printError(const stacklang::exceptions::LanguageException&) noexcept;

const int CURSOR_INVISIBLE = 0;
const int CURSOR_VISIBLE = 1;
const int CURSOR_VERY_VISIBLE = 2;

// const char KEY_WIN_RESIZE = 0;

const char* const INFO = R"(StackLang interpreter version 2
Copyright 2018 Justin Hu, Bronwyn Damm, Jacques Marais, Ramon Rakow, and
Jude Sidloski
This program comes with ABSOLUTELY NO WARRANTY. This is free software, and you
are welcome to redistribute it under certain conditions. For more detail see
the COPYING file that should have been included with this program.

Use ^D to exit from prompt, ^C to stop execution, and ^\ to force quit.
Press any key to continue...)";

const char* const HELPMSG = R"(Usage: stacklang [OPTIONS]
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
