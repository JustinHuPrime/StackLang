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

#include "language/stack/stackElements/substackElement.h"

#include "language/exceptions/parserException.h"
#include "language/stack.h"
#include "util/stringUtils.h"

namespace stacklang {
namespace stackelements {
using stacklang::StackElement;
using stacklang::exceptions::ParserException;
using std::to_string;
using util::trim;

const char* SubstackElement::SUBSTACK_BEGIN = "<<";
const char* SubstackElement::SUBSTACK_END = ">>";
const char* SubstackElement::SUBSTACK_SEPARATOR = ", ";
const char* SubstackElement::SUBSTACK_EMPTY = "<< (empty) >>";

SubstackElement* SubstackElement::parse(const string& s) {
  int parseLevel = 0;
  bool inString = false;
  string accumulator = "";
  char lastChar = '\0';
  Stack buffer;

  for (auto iter = s.begin() + 2; iter != s.end() - 2;
       iter++)  // loop between the << and >>'s
  {
    accumulator += *iter;

    if (!inString && *iter == ',' &&
        parseLevel == 0) {  // found a comma at top level - our responsibility
      accumulator.erase(accumulator.end() - 1);  // remove the comma
      buffer.push(StackElement::parse(trim(accumulator)));
      accumulator = "";
    } else if (*iter == '"' && lastChar != '\\') {  // found an unescaped quote
      inString = !inString;
    } else if (!inString && *iter == '<' &&
               lastChar == '<') {  // start of substack
      parseLevel++;
    } else if (!inString && *iter == '>' &&
               lastChar == '>') {  // end of substack
      parseLevel--;
      if (parseLevel < 0) {
        throw ParserException(
            "Missing at least one matching opening substack delimiter.", s,
            iter - s.begin() - 1);
      }
    }

    lastChar = *iter;
  }

  if (parseLevel != 0) {
    throw ParserException("Missing " + to_string(parseLevel) +
                              " closing substack delimiter" +
                              (parseLevel == 1 ? "" : "s") + ".",
                          s, s.length() - 1);
  } else if (trim(accumulator) !=
             "") {  // allow empty lists and terminal commas
    buffer.push(StackElement::parse(trim(accumulator)));
  }

  buffer.reverse();
  return new SubstackElement(buffer);
}

SubstackElement::SubstackElement(const Stack& s) noexcept
    : StackElement(StackElement::DataType::Substack), data(s) {}

SubstackElement* SubstackElement::clone() const noexcept {
  return new SubstackElement(data);
}

SubstackElement::operator string() const noexcept {
  if (data.size() == 0) {
    return SUBSTACK_EMPTY;
  }
  string buffer = SUBSTACK_BEGIN;
  buffer += " ";

  for (auto elm : data) {
    buffer += string(*elm);
    buffer += SUBSTACK_SEPARATOR;
  }

  buffer.erase(buffer.length() - 2);

  buffer += " ";
  buffer += SUBSTACK_END;

  return buffer;  // return value optimization
}

const Stack& SubstackElement::getData() const noexcept { return data; }
}  // namespace stackelements
}  // namespace stacklang