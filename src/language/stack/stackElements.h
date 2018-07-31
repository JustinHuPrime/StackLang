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

// Definition of stackElements for various types.

#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_

#include <string>
#include <vector>

#include <gmp.h>
#include <gmpxx.h>

#include "language/stack/stack.h"

namespace stacklang {
namespace stackelements {
namespace {
using stacklang::Stack;
using std::string;
using std::vector;
}  // namespace

class BooleanElement : public StackElement {
 public:
  static const char* TSTR;
  static const char* FSTR;

  explicit BooleanElement(bool) noexcept;
  BooleanElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  bool getData() const noexcept;

 private:
  bool data;
};

class CommandElement : public StackElement {
 public:
  static const char* ALLOWED_COMMAND;

  static CommandElement* parse(const string&);

  explicit CommandElement(const string&, bool isQuoted = false) noexcept;
  CommandElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  const string& getName() const noexcept;
  bool isQuoted() const noexcept;

  static const char QUOTE_CHAR;

 private:
  static const char* COMMAND_LDELIM;
  static const char* COMMAND_RDELIM;

  string name;
  bool quoted;
};

class NumberElement : public StackElement {
 public:
  static const char* ALLOWED_NUMBER;
  static const char* NUMBER_SIGNS;
  static const char INEXACT_SIGNAL;

  static NumberElement* parse(const string&);

  explicit NumberElement(string, bool isExact = true) noexcept;
  explicit NumberElement(const mpq_class&, bool isExact = true) noexcept;
  NumberElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  const mpq_class& getData() const noexcept;
  bool isExact() const noexcept;

 private:
  mpq_class data;
  bool exact;
};

class StringElement : public StackElement {
 public:
  static StringElement* parse(const string&);
  explicit StringElement(string) noexcept;
  StringElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  const string& getData() const noexcept;

  static const char QUOTE_CHAR;

 private:
  string data;
};

class SubstackElement : public StackElement {
 public:
  static SubstackElement* parse(const string&);
  explicit SubstackElement(const Stack&) noexcept;
  SubstackElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  const Stack& getData() const noexcept;

  static const char* SUBSTACK_BEGIN;
  static const char* SUBSTACK_END;

 private:
  static const char* SUBSTACK_SEPARATOR;
  static const char* SUBSTACK_EMPTY;

  Stack data;
};

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

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_