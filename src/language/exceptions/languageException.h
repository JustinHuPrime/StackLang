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

// Pure virtual base exception class used to signal interpreter error
// conditions. See other classes in this directory for specializations.

#ifndef STACKLANG_LANGUAGE_EXCEPTONS_LANGUAGEEXCEPTION_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_LANGUAGEEXCEPTION_H_

#include <exception>
#include <string>

namespace stacklang {
namespace exceptions {
using std::exception;
using std::string;

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
}  // namespace exceptions
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_LANGUAGEEXCEPTION_H_