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

#include "language/stack/stackElements/typeElement.h"

#include <algorithm>
#include <array>
#include <string>

#include "language/exceptions/parserException.h"
#include "util/stringUtils.h"

namespace stacklang {
namespace stackelements {
using stacklang::StackElement;
using stacklang::exceptions::ParserException;
using std::array;
using std::distance;
using std::find;
using std::string;
using util::starts_with;

const char* TypeElement::PARENS = "()";

TypeElement* TypeElement::parse(const string& s) {
  if (s.find('(') == string::npos) {
    auto value = find(TYPES().begin(), TYPES().end(), s);
    if (value == TYPES().end()) {
      throw ParserException("Input is not a type.", s, 0);
    }
    return new TypeElement(static_cast<DataType>(value - begin(TYPES())));
  } else if (starts_with(s, "Substack")) {  // substack specializations
    TypeElement* elm = new TypeElement(
        DataType::Substack,
        TypeElement::parse(s.substr(s.find_first_of('(') + 1,
                                    s.length() - s.find_first_of('(') - 2)));
    if (static_cast<unsigned>(elm->specialization->data) >= NUM_PRIM_TYPES) {
      delete elm;
      throw ParserException("Bad specialization on a Substack type.", s,
                            s.find('(') + 1);
    }
    return elm;
  } else if (starts_with(s, "Number")) {
    TypeElement* elm = new TypeElement(
        DataType::Number,
        TypeElement::parse(s.substr(s.find_first_of('(') + 1,
                                    s.length() - s.find_first_of('(') - 2)));
    if (elm->specialization->data != DataType::Exact &&
        elm->specialization->data != DataType::Inexact) {
      delete elm;
      throw ParserException("Bad specialization on a Number type.", s,
                            s.find('(') + 1);
    }
    return elm;
  } else if (starts_with(s, "Command")) {
    TypeElement* elm = new TypeElement(
        DataType::Command,
        TypeElement::parse(s.substr(s.find_first_of('(') + 1,
                                    s.length() - s.find_first_of('(') - 2)));
    if (elm->specialization->data != DataType::Quoted) {
      delete elm;
      throw ParserException("Wrong specialization on a Command type.", s,
                            s.find('(') + 1);
    }
    return elm;
  } else {
    throw ParserException(
        "Cannot have a specialzation except on a Substack, Number, or Command.",
        s, s.find('('));
  }
}  // namespace StackElements

TypeElement::TypeElement(DataType type) noexcept
    : StackElement(StackElement::DataType::Type),
      data(type),
      specialization(nullptr) {}

TypeElement::TypeElement(DataType type, TypeElement* subType) noexcept
    : StackElement(StackElement::DataType::Type),
      data(type),
      specialization(subType) {}

TypeElement::TypeElement(const TypeElement& other) noexcept
    : StackElement(StackElement::DataType::Type), data(other.data) {
  if (other.specialization == nullptr)
    specialization = nullptr;
  else
    specialization = other.specialization->clone();
}

TypeElement& TypeElement::operator=(const TypeElement& other) noexcept {
  data = other.data;
  if (specialization != nullptr) delete specialization;
  specialization =
      other.specialization == nullptr ? nullptr : other.specialization->clone();
  return *this;
}

TypeElement::TypeElement(TypeElement&& other) noexcept
    : StackElement(StackElement::DataType::Type), data(other.data) {
  TypeElement* temp = specialization;
  specialization = other.specialization;
  other.specialization = temp;
}

TypeElement& TypeElement::operator=(TypeElement&& other) noexcept {
  data = other.data;
  TypeElement* temp = specialization;
  specialization = other.specialization;
  other.specialization = temp;
  return *this;
}

TypeElement::~TypeElement() noexcept { delete specialization; }

TypeElement* TypeElement::clone() const noexcept {
  return new TypeElement(data, specialization);
}

TypeElement::operator string() const noexcept {
  if (specialization == nullptr)
    return to_string(data);
  else
    return to_string(data) + "(" + string(*specialization) + ")";
}

StackElement::DataType TypeElement::getData() const noexcept { return data; }
const TypeElement* TypeElement::getSpecialization() const noexcept {
  return specialization;
}

string TypeElement::to_string(StackElement::DataType type) noexcept {
  return TYPES()[static_cast<unsigned>(type)];
}

const vector<string>& TypeElement::TYPES() noexcept {
  static vector<string>* TYPES =
      new vector<string>{"Number",  "String", "Boolean", "Substack", "Type",
                         "Command", "Any",    "Exact",   "Inexact",  "Quoted"};
  return *TYPES;
}
}  // namespace stackelements
}  // namespace stacklang