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

#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "language/stack/stack.h"

namespace stacklang::stackelements {

class BooleanElement : public StackElement {
 public:
  static const char* const TSTR;
  static const char* const FSTR;

  explicit BooleanElement(bool) noexcept;
  BooleanElement* clone() const noexcept override;

  bool operator==(const StackElement&) const noexcept override;

  explicit operator std::string() const noexcept override;
  bool getData() const noexcept;

 private:
  bool data;
};

class CommandElement : public StackElement {
 public:
  CommandElement(bool) noexcept;
  CommandElement* clone() const noexcept override;

  bool operator==(const StackElement&) const noexcept override;

  explicit operator std::string() const noexcept override;

  bool isPrimitive() const noexcept;

 private:
  bool primtiive;
};

class PrimitiveCommandElement : public CommandElement {
 public:
  PrimitiveCommandElement(
      std::function<void(Stack&, Defines&,
                         std::vector<std::string>&)>) noexcept;
  PrimitiveCommandElement* clone() const noexcept override;

  bool operator==(const StackElement&) const noexcept override;

  explicit operator std::string() const noexcept override;

  void operator()(Stack&, Defines&, std::vector<std::string>&) const;

 private:
  std::function<void(Stack&, Defines&, std::vector<std::string>&)> primitive;
};

class DefinedCommandElement : public CommandElement {
 public:
  DefinedCommandElement(?) noexcept;
  DefinedCommandElement* clone() const noexcept override;

  bool operator==(const StackElement&) const noexcept override;

  explicit operator std::string() const noexcept override;

  void operator()(Stack&, Defines&, std::vector<std::string>&) const;

 private:
  std::function<void(Stack&, Defines&, std::vector<std::string>&)> primitive;
};

class IdentifierElement : public StackElement {
 public:
  static const char* const ALLOWED_IDENTIFIER;

  static IdentifierElement* parse(const std::string&);

  explicit IdentifierElement(const std::string&,
                             bool isQuoted = false) noexcept;
  IdentifierElement* clone() const noexcept override;

  bool operator==(const StackElement&) const noexcept override;

  explicit operator std::string() const noexcept override;
  const std::string& getName() const noexcept;
  bool isQuoted() const noexcept;

  static const char QUOTE_CHAR;

 private:
  std::string name;
  bool quoted;
};

class NumberElement : public StackElement {
 public:
  static const char* const ALLOWED_NUMBER;
  static const char* const NUMBER_SIGNS;

  static NumberElement* parse(const std::string&);

  explicit NumberElement(
      long double,
      int = std::numeric_limits<long double>::max_digits10) noexcept;
  explicit NumberElement(std::string) noexcept;
  NumberElement* clone() const noexcept override;

  bool operator==(const StackElement&) const noexcept override;

  explicit operator std::string() const noexcept override;
  long double getData() const noexcept;
  int getPrecision() const noexcept;

 private:
  long double data;
  int precision;
};

class StringElement : public StackElement {
 public:
  static const char QUOTE_CHAR;

  static StringElement* parse(const std::string&);
  explicit StringElement(std::string) noexcept;
  StringElement* clone() const noexcept override;

  bool operator==(const StackElement&) const noexcept override;

  explicit operator std::string() const noexcept override;
  const std::string& getData() const noexcept;

 private:
  std::string data;
};

class SubstackElement : public StackElement {
 public:
  static SubstackElement* parse(const std::string&);
  explicit SubstackElement(const Stack&) noexcept;
  SubstackElement* clone() const noexcept override;

  bool operator==(const StackElement&) const noexcept override;

  explicit operator std::string() const noexcept override;
  const Stack& getData() const noexcept;

  static const char* const SUBSTACK_BEGIN;
  static const char* const SUBSTACK_END;

 private:
  static const char* const SUBSTACK_SEPARATOR;
  static const char* const SUBSTACK_EMPTY;

  Stack data;
};

class TypeElement : public StackElement {
 public:
  static TypeElement* parse(const std::string&);

  explicit TypeElement(DataType, TypeElement* = nullptr) noexcept;
  TypeElement(const TypeElement&) noexcept;
  TypeElement& operator=(const TypeElement&) noexcept;
  TypeElement(TypeElement&&) noexcept;
  TypeElement& operator=(TypeElement&&) noexcept;
  TypeElement* clone() const noexcept override;
  ~TypeElement() noexcept;

  bool operator==(const StackElement&) const noexcept override;

  explicit operator std::string() const noexcept override;
  DataType getBase() const noexcept;
  const TypeElement* getSpecialization() const noexcept;

  static std::string to_string(DataType) noexcept;

  static const std::vector<std::string>& TYPES() noexcept;

  static const char* const PARENS;

 private:
  DataType data;
  TypeElement* specialization;
};

typedef std::unique_ptr<BooleanElement> BooleanPtr;
typedef std::unique_ptr<CommandElement> CommandPtr;
typedef std::unique_ptr<NumberElement> NumberPtr;
typedef std::unique_ptr<StringElement> StringPtr;
typedef std::unique_ptr<SubstackElement> SubstackPtr;
typedef std::unique_ptr<TypeElement> TypePtr;

}  // namespace stacklang::stackelements

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_
