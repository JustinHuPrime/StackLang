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

#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_

#include <string>
#include <vector>

#include "language/stack/stackElement.h"

namespace stacklang {
namespace stackelements {
using std::string;
using std::vector;
class TypeElement : public StackElement {
 public:
  static TypeElement* parse(const string&);
  explicit TypeElement(DataType) noexcept;
  TypeElement(DataType, TypeElement*) noexcept;
  TypeElement(const TypeElement&) noexcept;
  TypeElement& operator=(const TypeElement&) noexcept;
  TypeElement(TypeElement&&) noexcept;
  TypeElement& operator=(TypeElement&&) noexcept;
  TypeElement* clone() const noexcept override;
  ~TypeElement() noexcept;

  explicit operator string() const noexcept override;
  DataType getData() const noexcept;
  const TypeElement* getSpecialization() const noexcept;

  static string to_string(DataType) noexcept;

  static const vector<string>& TYPES() noexcept;

  static const char* PARENS;

 private:
  DataType data;
  TypeElement* specialization;
};
}  // namespace stackelements
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_