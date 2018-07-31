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

// Definitions of language-related errors. See interpreterExceptions.h for
// derived interpreter errors.

#ifndef STACKLANG_LANGUAGE_EXCEPTONS_LANGUAGEEXCEPTIONS_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_LANGUAGEEXCEPTIONS_H_

#include <string>

#include "language/stackElements.h"

namespace stacklang {
namespace exceptions {
namespace {
using stacklang::stackelements::TypeElement;
using std::string;
}  // namespace

// Describes a StackLang runtime error
class LanguageException {
 public:
  // Creates an error with a message, but no context
  explicit LanguageException(const string&);
  // Creates an error with a message and context (plus exact location of error)
  LanguageException(const string&, const string&, unsigned);
  LanguageException(const LanguageException&) = default;

  LanguageException& operator=(const LanguageException&) = default;

  // Get error details
  virtual const string getKind() const = 0;
  const string getMessage() const;
  const string getContext() const;
  unsigned getLocation() const;
  bool hasContext() const;

 protected:
  string message, context;
  unsigned location;
  bool errorHasContext;
};

class StackOverflowError : public LanguageException {
 public:
  explicit StackOverflowError(unsigned long);
  StackOverflowError(const StackOverflowError&) = default;

  StackOverflowError& operator=(const StackOverflowError&) = default;

  const string getKind() const override;
};

class StackUnderflowError : public LanguageException {
 public:
  StackUnderflowError();
  StackUnderflowError(const StackUnderflowError&) = default;

  StackUnderflowError& operator=(const StackUnderflowError&) = default;

  const string getKind() const override;
};

class StopError : public LanguageException {
 public:
  StopError();
  StopError(const StopError&) = default;

  StopError& operator=(const StopError&) = default;

  const string getKind() const override;
};

class SyntaxError : public LanguageException {
 public:
  SyntaxError(const string& msg);
  SyntaxError(const string& msg, const string& ctx, size_t pos);
  SyntaxError(const SyntaxError&) = default;

  SyntaxError& operator=(const SyntaxError&) = default;

  const string getKind() const override;
};

class TypeError : public LanguageException {
 public:
  TypeError(const StackElement&, const StackElement&);
  TypeError(const StackElement&);
  TypeError(const TypeError&) = default;

  TypeError& operator=(const TypeError&) = default;

  const string getKind() const override;
};
}  // namespace exceptions
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_LANGUAGEEXCEPTION_H_