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

#include "language/stack/stackElements/stringElement.h"

#include "language/exceptions/parserException.h"
#include "util/stringUtils.h"

namespace stacklang {
namespace stackelements {
using stacklang::exceptions::ParserException;
using util::ends_with;
using util::escape;
using util::findImproperEscape;
using util::unescape;

const char StringElement::QUOTE_CHAR = '"';

StringElement* StringElement::parse(const string& s) {
  if (ends_with(s, "\""))  // has quotes on either end
  {
    if (findImproperEscape(s.substr(1, s.length() - 2)) == string::npos) {
      return new StringElement(unescape(s.substr(1, s.length() - 2)));
    } else {
      throw ParserException(
          "Looks like a string, but has an invalid escape sequence", s,
          findImproperEscape(s));
    }
  } else {
    throw ParserException(
        "Looks like a string, but is missing a closing quote.", s, s.length());
  }
}

StringElement::StringElement(string s) noexcept
    : StackElement(StackElement::DataType::String), data(s) {}

StringElement* StringElement::clone() const noexcept {
  return new StringElement(data);
}

StringElement::operator string() const noexcept {
  return "\"" + escape(data) + "\"";
}

const string& StringElement::getData() const noexcept { return data; }
}  // namespace stackelements
}  // namespace stacklang