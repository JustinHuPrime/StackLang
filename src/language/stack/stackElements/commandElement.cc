// Copyright 2018 Justin Hu
//
// This file is part of the StackLang interpreter.
//
// The StackLang interpreter is free software: you can redistribute it and / or
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

#include "language/stack/stackElements/commandElement.h"

#include "language/exceptions/parserException.h"
#include "util/stringUtils.h"

namespace stacklang {
namespace stackelements {
using stacklang::exceptions::ParserException;
using util::removeChar;

const char* CommandElement::COMMAND_LDELIM = "<";
const char* CommandElement::COMMAND_RDELIM = ">";
const char* CommandElement::ALLOWED_COMMAND =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-?*";
const char CommandElement::QUOTE_CHAR = '`';

CommandElement* CommandElement::parse(const string& s) {
  if (s.find_first_not_of(CommandElement::ALLOWED_COMMAND, 1) == string::npos &&
      (isalpha(s[0]) ||
       (s[0] == QUOTE_CHAR && s.length() >= 2 &&
        isalpha(s[1]))))  // has only allowed characters, starts with a
                          // quote char and a letter or a letter
  {
    return new CommandElement(removeChar(s, QUOTE_CHAR), s[0] == QUOTE_CHAR);
  } else {
    unsigned badIndex = s.find_first_not_of(CommandElement::ALLOWED_COMMAND,
                                            s[0] == QUOTE_CHAR ? 1 : 0);
    if (s[badIndex] == ' ')  // has a space
    {
      throw ParserException("Input looks like a command, but has a space.", s,
                            badIndex);
    } else {
      throw ParserException(
          "Input looks like a command, but has a symbol "
          "that is not in `-?*`.",
          s, badIndex);
    }
  }
}

CommandElement::CommandElement(const string& s, bool isQuoted) noexcept
    : StackElement(StackElement::DataType::Command),
      data(s),
      quoted(isQuoted) {}

CommandElement* CommandElement::clone() const noexcept {
  return new CommandElement(data, quoted);
}

CommandElement::operator string() const noexcept {
  return (quoted ? string(1, QUOTE_CHAR) : "") + COMMAND_LDELIM + data +
         COMMAND_RDELIM;
}

const string& CommandElement::getData() const noexcept { return data; }
bool CommandElement::isQuoted() const noexcept { return quoted; }
}  // namespace stackelements
}  // namespace stacklang