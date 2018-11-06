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
#include <vector>

#include "language/stack/stackElements.h"

namespace stacklang::exceptions {

// Describes a StackLang runtime error
class LanguageException : public std::exception {
 public:
  // Creates an error with a message, but no context
  explicit LanguageException(
      const std::string&,
      std::vector<std::string> = std::vector<std::string>{}) noexcept;
  // Creates an error with a message and context (plus exact location of error)
  LanguageException(
      const std::string&, const std::string&, size_t,
      std::vector<std::string> = std::vector<std::string>{}) noexcept;
  LanguageException(const LanguageException&) = default;

  LanguageException& operator=(const LanguageException&) = default;

  // Get error details
  virtual std::string getKind() const noexcept = 0;
  const std::string& getMessage() const noexcept;
  const std::string& getContext() const noexcept;
  size_t getLocation() const noexcept;
  bool hasContext() const noexcept;
  const std::vector<std::string>& getTrace() const noexcept;

 protected:
  std::string message, context;
  size_t location;
  bool errorHasContext;
  std::vector<std::string> stacktrace;
};

class RuntimeError : public LanguageException {
 public:
  explicit RuntimeError(
      const std::string&,
      std::vector<std::string> = std::vector<std::string>{}) noexcept;
  RuntimeError(const std::string&, const std::string&, size_t,
               std::vector<std::string> = std::vector<std::string>{}) noexcept;
  RuntimeError(const RuntimeError&) = default;

  RuntimeError& operator=(const RuntimeError&) = default;

  std::string getKind() const noexcept override;
};

class StackOverflowError : public LanguageException {
 public:
  explicit StackOverflowError(
      size_t, std::vector<std::string> = std::vector<std::string>{}) noexcept;
  StackOverflowError(const StackOverflowError&) = default;

  StackOverflowError& operator=(const StackOverflowError&) = default;

  std::string getKind() const noexcept override;
};

class StackUnderflowError : public LanguageException {
 public:
  StackUnderflowError(
      std::vector<std::string> = std::vector<std::string>{}) noexcept;
  StackUnderflowError(const StackUnderflowError&) = default;

  StackUnderflowError& operator=(const StackUnderflowError&) = default;

  std::string getKind() const noexcept override;
};

class StopError : public LanguageException {
 public:
  explicit StopError(
      std::vector<std::string> = std::vector<std::string>{}) noexcept;
  StopError(const StopError&) = default;

  StopError& operator=(const StopError&) = default;

  std::string getKind() const noexcept override;
};

class SyntaxError : public LanguageException {
 public:
  SyntaxError(const std::string& msg,
              std::vector<std::string> = std::vector<std::string>{}) noexcept;
  SyntaxError(const std::string& msg, const std::string& ctx, size_t pos,
              std::vector<std::string> = std::vector<std::string>{}) noexcept;
  SyntaxError(const SyntaxError&) = default;

  SyntaxError& operator=(const SyntaxError&) = default;

  std::string getKind() const noexcept override;
};

class TypeError : public LanguageException {
 public:
  TypeError(const StackElement&, const StackElement&,
            std::vector<std::string> = std::vector<std::string>{}) noexcept;
  TypeError(const StackElement&,
            std::vector<std::string> = std::vector<std::string>{}) noexcept;
  TypeError(const TypeError&) = default;

  TypeError& operator=(const TypeError&) = default;

  std::string getKind() const noexcept override;
};
}  // namespace stacklang::exceptions

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_LANGUAGEEXCEPTION_H_
