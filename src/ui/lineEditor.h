#ifndef STACKLANG_UI_LINEEDITOR_H_
#define STACKLANG_UI_LINEEDITOR_H_

#include <ncurses.h>
#include <list>
#include <stack>
#include <string>

namespace TermUI {
using std::list;
using std::stack;
using std::string;

class LineEditor {
 public:
  // Create blank lineEditor
  LineEditor() noexcept;
  LineEditor(const LineEditor&) noexcept = default;
  LineEditor(LineEditor&&) noexcept = default;

  LineEditor& operator=(const LineEditor&) noexcept = default;
  LineEditor& operator=(LineEditor&&) noexcept = default;

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
  void operator+=(string) noexcept;

  explicit operator const string() const noexcept;

 private:
  list<char> preCursor;
  list<char> postCursor;

  list<string> history;
  list<string>::const_iterator histPos;
  string draftLine;
};
}  // namespace TermUI

#endif  // STACKLANG_UI_LINEEDITOR_H_