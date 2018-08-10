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

#include <list>
#include <string>

#include "language/stack/stackElements.h"

namespace stacklang {
namespace exceptions {
namespace {
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::TypeElement;
using std::list;
using std::string;
}  // namespace

// Describes a StackLang runtime error
class LanguageException {
 public:
  // Creates an error with a message, but no context
  explicit LanguageException(const string&,
                             list<string> = list<string>{}) noexcept;
  // Creates an error with a message and context (plus exact location of error)
  LanguageException(const string&, const string&, size_t,
                    list<string> = list<string>{}) noexcept;
  LanguageException(const LanguageException&) = default;

  LanguageException& operator=(const LanguageException&) = default;

  // Get error details
  virtual string getKind() const noexcept = 0;
  const string& getMessage() const noexcept;
  const string& getContext() const noexcept;
  size_t getLocation() const noexcept;
  bool hasContext() const noexcept;
  const list<string>& getTrace() const noexcept;

 protected:
  string message, context;
  size_t location;
  bool errorHasContext;
  list<string> stacktrace;
};

class RuntimeError : public LanguageException {
 public:
  explicit RuntimeError(const string&, list<string> = list<string>{}) noexcept;
  RuntimeError(const RuntimeError&) = default;

  RuntimeError& operator=(const RuntimeError&) = default;

  string getKind() const noexcept override;
};

class StackOverflowError : public LanguageException {
 public:
  explicit StackOverflowError(size_t, list<string> = list<string>{}) noexcept;
  StackOverflowError(const StackOverflowError&) = default;

  StackOverflowError& operator=(const StackOverflowError&) = default;

  string getKind() const noexcept override;
};

class StackUnderflowError : public LanguageException {
 public:
  StackUnderflowError(list<string> = list<string>{}) noexcept;
  StackUnderflowError(const StackUnderflowError&) = default;

  StackUnderflowError& operator=(const StackUnderflowError&) = default;

  string getKind() const noexcept override;
};

class StopError : public LanguageException {
 public:
  StopError(list<string> = list<string>{}) noexcept;
  StopError(const StopError&) = default;

  StopError& operator=(const StopError&) = default;

  string getKind() const noexcept override;
};

class SyntaxError : public LanguageException {
 public:
  SyntaxError(const string& msg, list<string> = list<string>{}) noexcept;
  SyntaxError(const string& msg, const string& ctx, size_t pos,
              list<string> = list<string>{}) noexcept;
  SyntaxError(const SyntaxError&) = default;

  SyntaxError& operator=(const SyntaxError&) = default;

  string getKind() const noexcept override;
};

class TypeError : public LanguageException {
 public:
  TypeError(const StackElement&, const StackElement&,
            list<string> = list<string>{}) noexcept;
  TypeError(const StackElement&, list<string> = list<string>{}) noexcept;
  TypeError(const TypeError&) = default;

  TypeError& operator=(const TypeError&) = default;

  string getKind() const noexcept override;
};
}  // namespace exceptions
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_LANGUAGEEXCEPTION_H_