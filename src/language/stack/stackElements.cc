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

// Implementation of stackElement for various types.

#include "language/stack/stackElements.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

#include "language/exceptions/interpreterExceptions.h"
#include "util/stringUtils.h"

namespace stacklang {
namespace stackelements {
namespace {
using stacklang::exceptions::ParserException;
using std::fixed;
using std::setprecision;
using std::stringstream;
using std::swap;
using std::to_string;
using util::ends_with;
using util::escape;
using util::findImproperEscape;
using util::removeChar;
using util::starts_with;
using util::trim;
using util::unescape;
}  // namespace

const char* const BooleanElement::TSTR = "true";
const char* const BooleanElement::FSTR = "false";

BooleanElement::BooleanElement(bool b) noexcept
    : StackElement(StackElement::DataType::Boolean), data(b) {}

BooleanElement* BooleanElement::clone() const noexcept {
  return new BooleanElement(data);
}

BooleanElement::operator string() const noexcept { return data ? TSTR : FSTR; }

bool BooleanElement::getData() const noexcept { return data; }

const char* const CommandElement::COMMAND_LDELIM = "<";
const char* const CommandElement::COMMAND_RDELIM = ">";
const char* const CommandElement::ALLOWED_COMMAND =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-?*";
const char CommandElement::QUOTE_CHAR = '`';

CommandElement* CommandElement::parse(const string& s) {
  if (s.find_first_not_of(CommandElement::ALLOWED_COMMAND, 1) == string::npos &&
      (isalpha(s[0]) ||
       (s[0] == QUOTE_CHAR && s.length() >= 2 &&
        isalpha(s[1])))) {  // has only allowed characters, starts with a
                            // quote char and a letter or a letter
    return new CommandElement(removeChar(s, QUOTE_CHAR), s[0] == QUOTE_CHAR);
  } else {
    size_t badIndex = s.find_first_not_of(CommandElement::ALLOWED_COMMAND,
                                          s[0] == QUOTE_CHAR ? 1 : 0);
    if (s[badIndex] == ' ') {  // has a space
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
      name(s),
      quoted(isQuoted) {}

CommandElement* CommandElement::clone() const noexcept {
  return new CommandElement(name, quoted);
}

CommandElement::operator string() const noexcept {
  return (quoted ? string(1, QUOTE_CHAR) : "") + COMMAND_LDELIM + name +
         COMMAND_RDELIM;
}

const string& CommandElement::getName() const noexcept { return name; }
bool CommandElement::isQuoted() const noexcept { return quoted; }

const char* const NumberElement::ALLOWED_NUMBER = "~-+/1234567890.'";
const char* const NumberElement::NUMBER_SIGNS = "-+";
const char NumberElement::INEXACT_SIGNAL = '~';

NumberElement* NumberElement::parse(const string& s) {
  if (s.find_first_not_of(ALLOWED_NUMBER) ==
      string::npos) {  // has allowed chars
    if (count(s.begin(), s.end(), '.') + count(s.begin(), s.end(), '/') <=
        1) {  // only one dot or decimal point
      if (count(s.begin(), s.end(),
                INEXACT_SIGNAL) <= 1) {  // only one inexact signal
        if (s.find_last_of(INEXACT_SIGNAL) == 0 ||
            s.find_last_of(INEXACT_SIGNAL) ==
                string::npos) {  // and it's at the start, if anywhere
          if (s.find_last_of(NUMBER_SIGNS) == 0 ||
              (removeChar(s, '\'').find_last_of(NUMBER_SIGNS) == 1 &&
               s.find_last_of(INEXACT_SIGNAL) == 0) ||
              s.find_last_of(NumberElement::NUMBER_SIGNS) ==
                  string::npos) {  // sign is first, second but has a tilde
                                   // before it, or doesn't exist
            if (removeChar(s, '\'').find_first_of('/') !=
                removeChar(s, '\'').length() -
                    1) {  // doesn't have a blank denominator
              if (s.find_first_of('/') == string::npos ||
                  s.substr(s.find_first_of('/') + 1).find_first_not_of("0'") !=
                      string::npos) {  // doesn't have all zeroes in the
                                       // denominator.
                return new NumberElement(
                    removeChar(removeChar(s, '\''), INEXACT_SIGNAL),
                    s.find_first_of(INEXACT_SIGNAL) == string::npos);
              } else {
                throw ParserException(
                    "Fractions may not have a zero in the denominator.", s,
                    s.find_first_of('0', s.find_first_of('/')));
              }
            } else {
              throw ParserException(
                  "Looks like a fraction, but has an empty denominator.", s,
                  s.find_last_of('/'));
            }
          } else {
            throw ParserException(
                "Looks like a number, but has a sign in the middle.", s,
                s.find_last_of(NUMBER_SIGNS));
          }
        } else {
          throw ParserException(
              "Looks like a number, but has a tilde in the middle.", s,
              s.find_last_of(INEXACT_SIGNAL));
        }
      } else {
        throw ParserException(
            "Looks like a number, but has more than one tilde.", s,
            s.find(INEXACT_SIGNAL, s.find(INEXACT_SIGNAL) + 1));
      }
    } else {
      if (count(s.begin(), s.end(), '.') > 1) {  // more than one dot
        throw ParserException(
            "Looks like a number, but has more than one deminal point.", s,
            s.find('.', s.find('.') + 1));
      } else if (count(s.begin(), s.end(), '/') >
                 1) {  // has more than one slash
        throw ParserException(
            "Looks like a number, but has more than one fraction bar.", s,
            s.find('/', s.find('/') + 1));
      } else {  // dot and slash
        throw ParserException(
            "Looks like a number, but has a decimal point in a fraction.", s,
            s.find_first_of("."));
      }
    }
  } else {
    throw ParserException(
        "Looks like a number, but has an unexpected character.", s,
        s.find_first_not_of(NumberElement::ALLOWED_NUMBER));
  }
}

NumberElement::NumberElement(string d, bool isExact) noexcept
    : StackElement(StackElement::DataType::Number), exact(isExact) {
  d = removeChar(d, '+');
  if (d.find('.') == string::npos) {
    data = mpq_class(d, 10);
  } else {
    int exponent = d.length() - 1 - d.find('.');
    data = mpq_class(d.erase(d.find('.'), 1));
    for (int i = 0; i < exponent; i++) {
      data /= 10;
    }
  }
  data.canonicalize();
}

NumberElement::NumberElement(const mpq_class& d, bool isExact) noexcept
    : StackElement(StackElement::DataType::Number), data(d), exact(isExact) {}

NumberElement* NumberElement::clone() const noexcept {
  return new NumberElement(data.get_str(), exact);
}

NumberElement::operator string() const noexcept {
  if (exact) {
    return data.get_str();
  } else {
    stringstream stream;
    stream << fixed << setprecision(data.get_prec()) << data.get_d();
    string inexactSig;
    inexactSig += INEXACT_SIGNAL;
    return inexactSig + stream.str() + "...";
  }
}

const mpq_class& NumberElement::getData() const noexcept { return data; }
bool NumberElement::isExact() const noexcept { return exact; }

const char StringElement::QUOTE_CHAR = '"';

StringElement* StringElement::parse(const string& s) {
  if (ends_with(s, "\"")) {  // has quotes on either end
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

const char* const SubstackElement::SUBSTACK_BEGIN = "<<";
const char* const SubstackElement::SUBSTACK_END = ">>";
const char* const SubstackElement::SUBSTACK_SEPARATOR = ", ";
const char* const SubstackElement::SUBSTACK_EMPTY = "<< (empty) >>";

SubstackElement* SubstackElement::parse(const string& s) {
  int parseLevel = 0;
  bool inString = false;
  string accumulator = "";
  char lastChar = '\0';
  Stack buffer;

  for (auto iter = s.begin() + 2; iter != s.end() - 2;
       iter++) {  // loop between the << and >>'s
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
            iter - s.begin() -
                1);  // iter is always at least 2 greater than s.begin().
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
    buffer += static_cast<string>(*elm);
    buffer += SUBSTACK_SEPARATOR;
  }

  buffer.erase(buffer.length() - 2);

  buffer += " ";
  buffer += SUBSTACK_END;

  return buffer;  // return value optimization
}

const Stack& SubstackElement::getData() const noexcept { return data; }

const char* const TypeElement::PARENS = "()";

TypeElement* TypeElement::parse(const string& s) {
  if (s.find('(') == string::npos) {
    auto value = find(TYPES().begin(), TYPES().end(), s);
    if (value == TYPES().end()) {
      throw ParserException("Input is not a type.", s, 0);
    }
    return new TypeElement(static_cast<DataType>(value - begin(TYPES())));
  } else if (starts_with(s, "Substack")) {  // substack specializations
    TypePtr elm(new TypeElement(
        DataType::Substack,
        TypeElement::parse(s.substr(s.find_first_of('(') + 1,
                                    s.length() - s.find_first_of('(') - 2))));
    if (static_cast<int>(elm->specialization->data) >= NUM_PRIM_TYPES) {
      throw ParserException("Bad specialization on a Substack type.", s,
                            s.find('(') + 1);
    }
    return elm.release();
  } else if (starts_with(s, "Number")) {
    TypePtr elm(new TypeElement(
        DataType::Number,
        TypeElement::parse(s.substr(s.find_first_of('(') + 1,
                                    s.length() - s.find_first_of('(') - 2))));
    if (elm->specialization->data != DataType::Exact &&
        elm->specialization->data != DataType::Inexact) {
      throw ParserException("Bad specialization on a Number type.", s,
                            s.find('(') + 1);
    }
    return elm.release();
  } else if (starts_with(s, "Command")) {
    TypePtr elm(new TypeElement(
        DataType::Command,
        TypeElement::parse(s.substr(s.find_first_of('(') + 1,
                                    s.length() - s.find_first_of('(') - 2))));
    if (elm->specialization->data != DataType::Quoted &&
        elm->specialization->data != DataType::Unquoted) {
      throw ParserException("Wrong specialization on a Command type.", s,
                            s.find('(') + 1);
    }
    return elm.release();
  } else {
    throw ParserException(
        "Cannot have a specialzation except on a Substack, Number, or Command.",
        s, s.find('('));
  }
}  // namespace StackElements

TypeElement::TypeElement(DataType type, TypeElement* subType) noexcept
    : StackElement(StackElement::DataType::Type),
      data(type),
      specialization(subType) {}

TypeElement::TypeElement(const TypeElement& other) noexcept
    : StackElement(StackElement::DataType::Type), data(other.data) {
  specialization =
      other.specialization == nullptr ? nullptr : other.specialization->clone();
}

TypeElement& TypeElement::operator=(const TypeElement& other) noexcept {
  data = other.data;
  if (specialization != nullptr) delete specialization;
  specialization =
      other.specialization == nullptr ? nullptr : other.specialization->clone();
  return *this;
}

TypeElement::TypeElement(TypeElement&& other) noexcept
    : StackElement(StackElement::DataType::Type),
      data(other.data),
      specialization(nullptr) {
  swap(specialization, other.specialization);
}

TypeElement& TypeElement::operator=(TypeElement&& other) noexcept {
  data = other.data;
  swap(specialization, other.specialization);
  return *this;
}

TypeElement::~TypeElement() noexcept {
  if (specialization != nullptr) delete specialization;
}

TypeElement* TypeElement::clone() const noexcept {
  return new TypeElement(
      data, specialization == nullptr ? nullptr : specialization->clone());
}

TypeElement::operator string() const noexcept {
  if (specialization == nullptr)
    return to_string(data);
  else
    return to_string(data) + "(" + static_cast<string>(*specialization) + ")";
}

StackElement::DataType TypeElement::getData() const noexcept { return data; }
const TypeElement* TypeElement::getSpecialization() const noexcept {
  return specialization;
}

string TypeElement::to_string(StackElement::DataType type) noexcept {
  return TYPES()[static_cast<size_t>(type)];
}

const vector<string>& TypeElement::TYPES() noexcept {
  static vector<string>* TYPES =
      new vector<string>{"Number",  "String", "Boolean", "Substack", "Type",
                         "Command", "Any",    "Exact",   "Inexact",  "Quoted"};
  return *TYPES;
}
}  // namespace stackelements
}  // namespace stacklang