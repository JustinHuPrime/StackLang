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

// Implementation of full line editor with history

#include "ui/lineEditor.h"

namespace terminalui {
LineEditor::LineEditor() noexcept
    : preCursor(),
      postCursor(),
      history(),
      histPos(history.end()),
      draftLine() {}

void LineEditor::right() noexcept {
  if (postCursor.size() == 0) {
    beep();
    return;
  }

  preCursor.push_back(postCursor.front());
  postCursor.pop_front();
}

void LineEditor::left() noexcept {
  if (preCursor.size() == 0) {
    beep();
    return;
  }

  postCursor.push_front(preCursor.back());
  preCursor.pop_back();
}

void LineEditor::toEnd() noexcept {
  if (postCursor.size() == 0) {
    beep();
    return;
  }

  while (postCursor.size() > 0) {
    preCursor.push_back(postCursor.front());
    postCursor.pop_front();
  }
}

void LineEditor::toHome() noexcept {
  if (preCursor.size() == 0) {
    beep();
    return;
  }

  while (preCursor.size() > 0) {
    postCursor.push_front(preCursor.back());
    preCursor.pop_back();
  }
}

void LineEditor::up() noexcept {
  if (histPos == history.begin() || history.empty()) {
    beep();
    return;
  } else {
    if (histPos == history.end()) draftLine = static_cast<string>(*this);
    --histPos;
  }

  preCursor.clear();
  postCursor.clear();
  for (auto c : *histPos) {
    preCursor.push_back(c);
  }
}

void LineEditor::down() noexcept {
  if (histPos == history.end()) {
    beep();
    return;
  } else {
    ++histPos;
    preCursor.clear();
    postCursor.clear();
    if (histPos == history.end()) {
      for (auto c : draftLine) {
        preCursor.push_back(c);
      }
    } else {
      for (auto c : *histPos) {
        preCursor.push_back(c);
      }
    }
  }
}

void LineEditor::enter() noexcept {
  history.push_back(static_cast<string>(*this));
  histPos = history.end();
  draftLine.clear();
  preCursor.clear();
  postCursor.clear();
}

void LineEditor::backspace() noexcept {
  if (preCursor.size() == 0) {
    beep();
    return;
  }
  preCursor.pop_back();
}

void LineEditor::del() noexcept {
  if (postCursor.size() == 0) {
    beep();
    return;
  }
  postCursor.pop_front();
}

void LineEditor::clear() noexcept {
  preCursor.clear();
  postCursor.clear();
}

void LineEditor::clearHist() noexcept { history.clear(); }

int LineEditor::cursorPosition() const noexcept { return preCursor.size(); }

bool LineEditor::isEmpty() const noexcept {
  return preCursor.empty() && postCursor.empty();
}

void LineEditor::operator+=(char c) noexcept { preCursor.push_back(c); }

void LineEditor::operator+=(string s) noexcept {
  for (auto c : s) {
    preCursor.push_back(c);
  }
}

LineEditor::operator const string() const noexcept {
  string temp;

  for (auto c : preCursor) {
    temp += c;
  }

  for (auto c : postCursor) {
    temp += c;
  }

  return temp;
}
}  // namespace terminalui