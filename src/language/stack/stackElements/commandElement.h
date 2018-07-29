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

#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_COMMANDELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_COMMANDELEMENT_H_

#include <string>

#include "language/stack/stackElement.h"

namespace stacklang {
namespace stackelements {
using std::string;

class CommandElement : public StackElement {
 public:
  static const char* ALLOWED_COMMAND;

  static CommandElement* parse(const string&);

  explicit CommandElement(const string&, bool isQuoted = false) noexcept;
  CommandElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  const string& getData() const noexcept;
  bool isQuoted() const noexcept;

  static const char QUOTE_CHAR;

 private:
  static const char* COMMAND_LDELIM;
  static const char* COMMAND_RDELIM;

  string data;
  bool quoted;
};
}  // namespace stackelements
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_COMMANDELEMENT_H_