#ifndef STACKLANG_UI_LINEEDITOR_H_
#define STACKLANG_UI_LINEEDITOR_H_

#include <ncurses.h>
#include <list>
#include <stack>
#include <string>

namespace Util {
using std::list;
using std::stack;
using std::string;

class LineEditor {
 public:
  // Create blank lineEditor
  LineEditor();
  LineEditor(const LineEditor&) = default;
  LineEditor(LineEditor&&) = default;

  LineEditor& operator=(const LineEditor&) = default;
  LineEditor& operator=(LineEditor&&) = default;

  // Move the cursor
  void right();
  void left();
  void toEnd();
  void toHome();

  // Scroll through the command history - cursor defaults to right side.
  void up();
  void down();

  // Add something to the history
  void enter();

  // Delete characters to the left or right of cursor
  void backspace();
  void del();

  // Clears the current line
  void clear();

  // Clears the history
  void clearHist();

  int cursorPosition() const;
  bool isEmpty() const;

  // Add a character or a string to the editor
  void operator+=(char);
  void operator+=(string);

  explicit operator const string() const;

 private:
  list<char> preCursor;
  list<char> postCursor;

  stack<string> preHistory;
  stack<string> postHistory;
};
}  // namespace Util

#endif  // STACKLANG_UI_LINEEDITOR_H_