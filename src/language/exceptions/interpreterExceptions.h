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

// Definition of interpreter level errors and exceptions

#ifndef STACKLANG_LANGUAGE_EXCEPTONS_INTERPRETEREXCEPTIONS_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_INTERPRETEREXCEPTIONS_H_

#include "language/exceptions/languageExceptions.h"

namespace stacklang {
namespace exceptions {
class ArgumentError : public LanguageException {
 public:
  ArgumentError(const string& msg) noexcept;
  ArgumentError(const string& msg, const string& actual,
                size_t posDif) noexcept;
  ArgumentError(const ArgumentError&) = default;

  ArgumentError& operator=(const ArgumentError&) = default;

  string getKind() const noexcept override;
};

class ParserException : public LanguageException {
 public:
  ParserException(const string&, const string&, size_t,
                  list<string> = list<string>{}) noexcept;
  ParserException(const ParserException&) = default;

  ParserException& operator=(const ParserException&) = default;

  string getKind() const noexcept override;
};
}  // namespace exceptions
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_INTERPRETERERROR_H_