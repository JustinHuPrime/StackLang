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

#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <utility>

#include "language/exceptions/interpreterExceptions.h"
#include "language/language.h"
#include "util/stringUtils.h"

namespace stacklang::stackelements {
namespace {
using stacklang::exceptions::ParserException;
using stacklang::exceptions::StackOverflowError;
using stacklang::exceptions::StackUnderflowError;
using std::count;
using std::fixed;
using std::make_unique;
using std::numeric_limits;
using std::pair;
using std::setprecision;
using std::stold;
using std::string;
using std::stringstream;
using std::swap;
using std::to_string;
using std::vector;
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

bool BooleanElement::operator==(const StackElement& elm) const noexcept {
  if (elm.getType() != dataType) {
    return false;
  } else {
    const BooleanElement& b = static_cast<const BooleanElement&>(elm);
    return b.data == data;
  }
}

BooleanElement::operator string() const noexcept { return data ? TSTR : FSTR; }

bool BooleanElement::getData() const noexcept { return data; }

CommandElement::CommandElement(bool prim) noexcept
    : StackElement(StackElement::DataType::Command), primitive(prim) {}

bool CommandElement::isPrimitive() const noexcept { return primitive; }

bool CommandElement::operator==(const StackElement& elm) const noexcept {
  return &elm == this;  // equality doesn't make sense for function values.
}

const char* const PrimitiveCommandElement::DISPLAY_AS = "<PRIMITIVE>";

PrimitiveCommandElement::PrimitiveCommandElement(
    std::function<void(Stack&, Environment&)> p) noexcept
    : CommandElement{true}, fun{p} {}
PrimitiveCommandElement* PrimitiveCommandElement::clone() const noexcept {
  return new PrimitiveCommandElement(fun);
}

PrimitiveCommandElement::operator std::string() const noexcept {
  return DISPLAY_AS;
}

void PrimitiveCommandElement::operator()(Stack& s, Environment& e) const {
  fun(s, e);
}

const char* const DefinedCommandElement::DISPLAY_AS = "<FUNCTION>";

DefinedCommandElement::DefinedCommandElement(const Stack& s, const Stack& b,
                                             const Environment& e) noexcept
    : CommandElement{false}, sig{s}, body{b} {
  env = Environment();
  for (const auto& layer : e) {
    env.emplace_back();
    for (const auto& entry : layer) {
      env.back().insert(pair<string, ElementPtr>(
          entry.first, ElementPtr(entry.second->clone())));
    }
  }
}
DefinedCommandElement* DefinedCommandElement::clone() const noexcept {
  return new DefinedCommandElement(sig, body, env);
}

DefinedCommandElement::operator std::string() const noexcept {
  return DISPLAY_AS;
}

void DefinedCommandElement::operator()(Stack& mainStack) {
  checkTypes(mainStack, sig);

  for (const auto& elm : body) {
    try {
      mainStack.push(elm->clone());
    } catch (const StackOverflowError& exn) {
      // stack error without trace must be main stack.
      if (exn.getTrace().empty()) {
        throw StackOverflowError(mainStack.getLimit());
      } else {
        throw;
      }
    } catch (const StackUnderflowError& exn) {
      if (exn.getTrace().empty()) {
        throw StackUnderflowError();
      } else {
        throw;
      }
    }
    execute(mainStack, env);
  }
}

const char* const IdentifierElement::ALLOWED_IDENTIFIER =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-?*";
const char IdentifierElement::QUOTE_CHAR = '`';

IdentifierElement* IdentifierElement::parse(const string& s) {
  if (s.find_first_not_of(IdentifierElement::ALLOWED_IDENTIFIER, 1) ==
          string::npos &&
      (isalpha(s[0]) ||
       (s[0] == QUOTE_CHAR && s.length() >= 2 &&
        isalpha(s[1])))) {  // has only allowed characters, starts with a
                            // quote char and a letter or a letter
    return new IdentifierElement(removeChar(s, QUOTE_CHAR), s[0] == QUOTE_CHAR);
  } else {
    size_t badIndex = s.find_first_not_of(IdentifierElement::ALLOWED_IDENTIFIER,
                                          s[0] == QUOTE_CHAR ? 1 : 0);
    if (badIndex == string::npos) {
      if (!(isalpha(s[0]) || ((s[0] == QUOTE_CHAR && isalpha(s[1]))))) {
        throw ParserException(
            "Input does not begin with an alphabetic character.", s,
            s[0] == QUOTE_CHAR ? 1 : 0);
      } else {
        throw ParserException("Input is too short.", s,
                              s[0] == QUOTE_CHAR ? 2 : 1);
      }
    } else if (s[badIndex] == ' ') {  // has a space
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

IdentifierElement::IdentifierElement(const string& s, bool isQuoted) noexcept
    : StackElement(StackElement::DataType::Identifier),
      name(s),
      quoted(isQuoted) {}

IdentifierElement* IdentifierElement::clone() const noexcept {
  return new IdentifierElement(name, quoted);
}

bool IdentifierElement::operator==(const StackElement& elm) const noexcept {
  if (elm.getType() != dataType) {
    return false;
  } else {
    const IdentifierElement& cmd = static_cast<const IdentifierElement&>(elm);
    return cmd.name == name && cmd.quoted == quoted;
  }
}

IdentifierElement::operator string() const noexcept {
  return (quoted ? string(1, QUOTE_CHAR) : "") + name;
}

const string& IdentifierElement::getName() const noexcept { return name; }
bool IdentifierElement::isQuoted() const noexcept { return quoted; }

const char* const NumberElement::ALLOWED_NUMBER = "-+1234567890.'";
const char* const NumberElement::NUMBER_SIGNS = "-+";

NumberElement* NumberElement::parse(const string& s) {
  if (s.find_first_not_of(ALLOWED_NUMBER) ==
      string::npos) {                           // has allowed chars
    if (count(s.begin(), s.end(), '.') <= 1) {  // only one dot or decimal point
      if (s.find_last_of(NUMBER_SIGNS) == 0 ||
          s.find_last_of(NumberElement::NUMBER_SIGNS) ==
              string::npos) {  // sign is first, second but has a tilde
                               // before it, or doesn't exist
        return new NumberElement(removeChar(s, '\''));
      } else {
        throw ParserException(
            "Looks like a number, but has a sign in the middle.", s,
            s.find_last_of(NUMBER_SIGNS));
      }
    } else {
      throw ParserException(
          "Looks like a number, but has more than one deminal point.", s,
          s.find('.', s.find('.') + 1));
    }
  } else {
    throw ParserException(
        "Looks like a number, but has an unexpected character.", s,
        s.find_first_not_of(NumberElement::ALLOWED_NUMBER));
  }
}

NumberElement::NumberElement(long double num, int prec) noexcept
    : StackElement(StackElement::DataType::Number),
      data(num),
      precision(prec) {}

NumberElement::NumberElement(string d) noexcept
    : StackElement(StackElement::DataType::Number), data(stold(d)) {
  precision =
      d.find('.') == string::npos ? 0 : d.substr(d.find('.') + 1).length();
}

NumberElement* NumberElement::clone() const noexcept {
  return new NumberElement(data, precision);
}

bool NumberElement::operator==(const StackElement& elm) const noexcept {
  if (elm.getType() != dataType) {
    return false;
  } else {
    const NumberElement& num = static_cast<const NumberElement&>(elm);
    return num.data == data && num.precision == precision;
  }
}

NumberElement::operator string() const noexcept {
  stringstream stream;
  stream << fixed << setprecision(precision) << data;
  return stream.str();
}

long double NumberElement::getData() const noexcept { return data; }
int NumberElement::getPrecision() const noexcept { return precision; }

const char StringElement::QUOTE_CHAR = '"';

StringElement* StringElement::parse(const string& s) {
  if (ends_with(s, "\"")) {  // has quotes on either end
    if (findImproperEscape(s.substr(1, s.length() - 2)) == string::npos) {
      return new StringElement(unescape(s.substr(1, s.length() - 2)));
    } else {
      throw ParserException(
          "Looks like a string, but has an invalid escape sequence", s,
          findImproperEscape(s.substr(1, s.length() - 2)) + 1);
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

bool StringElement::operator==(const StackElement& elm) const noexcept {
  if (elm.getType() != dataType) {
    return false;
  } else {
    const StringElement& str = static_cast<const StringElement&>(elm);
    return str.data == data;
  }
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
  bool setLastChar = true;
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
    } else if (inString && *iter == '\\' &&
               lastChar == '\\') {  // found an escaped backslash
      lastChar = '\0';
      setLastChar = false;
    } else if (!inString && *iter == '<' &&
               lastChar == '<') {  // start of substack
      lastChar = '\0';
      setLastChar = false;
      parseLevel++;
    } else if (!inString && *iter == '>' &&
               lastChar == '>') {  // end of substack
      lastChar = '\0';
      setLastChar = false;
      parseLevel--;
      if (parseLevel < 0) {
        throw ParserException(
            "Missing at least one matching opening substack delimiter.", s,
            static_cast<size_t>(iter - s.begin() - 1));
      }
    }

    if (setLastChar) lastChar = *iter;
    setLastChar = true;
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
    : StackElement(StackElement::DataType::Substack), data(s) {
  data.setLimit(numeric_limits<size_t>::max());
}

SubstackElement* SubstackElement::clone() const noexcept {
  return new SubstackElement(data);
}

bool SubstackElement::operator==(const StackElement& elm) const noexcept {
  if (elm.getType() != dataType) {
    return false;
  } else {
    const SubstackElement& s = static_cast<const SubstackElement&>(elm);
    if (s.data.size() != data.size()) return false;
    auto iter1 = s.data.begin(), iter2 = data.begin();
    for (; iter2 != data.end(); ++iter1, ++iter2) {
      if (!(**iter1 == **iter2)) return false;
    }
    return true;
  }
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
    return new TypeElement(
        DataType::Substack,
        TypeElement::parse(s.substr(s.find_first_of('(') + 1,
                                    s.length() - s.find_first_of('(') - 2)));
  } else if (starts_with(s, "Command")) {  // command specializations
    return new TypeElement(
        DataType::Command,
        TypeElement::parse(s.substr(s.find_first_of('(') + 1,
                                    s.length() - s.find_first_of('(') - 2)));
  } else if (starts_with(s, "Identifier")) {  // identifier specializations
    return new TypeElement(
        DataType::Identifier,
        TypeElement::parse(s.substr(s.find_first_of('(') + 1,
                                    s.length() - s.find_first_of('(') - 2)));
  } else {
    throw ParserException(
        "Cannot have a specialization except on a Substack, Command, or "
        "Identifier.",
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

bool TypeElement::operator==(const StackElement& elm) const noexcept {
  if (elm.getType() != dataType) {
    return false;
  } else {
    const TypeElement& t = static_cast<const TypeElement&>(elm);
    if (t.data != data) return false;
    if ((t.specialization == nullptr) != (specialization == nullptr))
      return false;
    return specialization == nullptr || *t.specialization == *specialization;
  }
}

TypeElement::operator string() const noexcept {
  if (specialization == nullptr)
    return to_string(data);
  else
    return to_string(data) + "(" + static_cast<string>(*specialization) + ")";
}

StackElement::DataType TypeElement::getBase() const noexcept { return data; }
const TypeElement* TypeElement::getSpecialization() const noexcept {
  return specialization;
}

string TypeElement::to_string(StackElement::DataType type) noexcept {
  return TYPES()[static_cast<size_t>(type)];
}

const vector<string>& TypeElement::TYPES() noexcept {
  static vector<string>* TYPES = new vector<string>{
      "Number",  "String",     "Boolean",   "Substack", "Type",
      "Command", "Identifier", "Primitive", "Defined",  "Any"};
  return *TYPES;
}
}  // namespace stacklang::stackelements