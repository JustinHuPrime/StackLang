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

// Full line editor with history (UI parts not included).

#ifndef STACKLANG_UI_LINEEDITOR_H_
#define STACKLANG_UI_LINEEDITOR_H_

#include <ncurses.h>
#include <list>
#include <stack>
#include <string>
#include <vector>

namespace terminalui {
class LineEditor {
 public:
  // Create blank lineEditor
  LineEditor() noexcept;
  LineEditor(const LineEditor&) = default;
  LineEditor(LineEditor&&) = default;

  LineEditor& operator=(const LineEditor&) = default;
  LineEditor& operator=(LineEditor&&) = default;

  // Move the cursor
  void right() noexcept;
  void left() noexcept;
  void toEnd() noexcept;
  void toHome() noexcept;

  // Scroll through the command history - cursor defaults to right side.
  void up() noexcept;
  void down() noexcept;

  // Add something to the history
  void enter() noexcept;

  // Delete characters to the left or right of cursor
  void backspace() noexcept;
  void del() noexcept;

  // Clears the current line
  void clear() noexcept;

  // Clears the history
  void clearHist() noexcept;

  int cursorPosition() const noexcept;
  bool isEmpty() const noexcept;

  // Add a character or a string to the editor
  void operator+=(char) noexcept;
  void operator+=(std::string) noexcept;

  explicit operator const std::string() const noexcept;

 private:
  std::list<char> preCursor;
  std::list<char> postCursor;

  std::list<std::string> history;
  std::list<std::string>::const_iterator histPos;
  std::string draftLine;
};
}  // namespace terminalui

#endif  // STACKLANG_UI_LINEEDITOR_H_
