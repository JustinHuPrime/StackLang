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

// Pure virtual element on the stack. See stackElements folder for specific
// elements.

#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_

#include <string>

namespace stacklang {
using std::string;

// Represents a element in the stack. Subclassed to make a specific element.
class StackElement {
 public:
  static const unsigned NUM_PRIM_TYPES;
  enum class DataType {
    Number,
    String,
    Boolean,
    Substack,
    Type,
    Command,
    Any,
    Exact,
    Inexact,
    Quoted,
  };

  // Produces a StackElement (of some type) from a terminal input string
  static StackElement* parse(const string&);

  StackElement(const StackElement&) noexcept = default;
  StackElement(StackElement&&) noexcept = default;
  virtual StackElement* clone() const noexcept = 0;

  StackElement& operator=(const StackElement&) noexcept = default;
  StackElement& operator=(StackElement&&) noexcept = default;

  virtual ~StackElement() noexcept = default;

  // Produces a nicely formatted string of the string (for print to console)
  explicit virtual operator string() const noexcept = 0;

  // Getter for the DataType
  DataType getType() const noexcept;

  // Gets the data from the element (returns different type depending on
  // dataType) should exist in all subclasses, where T is the closest
  // dataTYpe to the element's type T getData () const;

  // Constructs an element of some type (takes different param type
  // depending on dataType) should exist in all subclasses. Element
  // (dataType);

 protected:
  // Constructs a StackElement by setting the dataType
  explicit StackElement(DataType) noexcept;

  DataType dataType;
};
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_