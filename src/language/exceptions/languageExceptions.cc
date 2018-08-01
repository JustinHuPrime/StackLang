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

// Implemetation of language errors

#include "language/exceptions/languageExceptions.h"

namespace stacklang {
namespace exceptions {
namespace {
using std::to_string;
}

LanguageException::LanguageException(const string& msg) noexcept
    : message(msg), errorHasContext(false) {}

LanguageException::LanguageException(const string& msg, const string& ctx,
                                     unsigned loc) noexcept
    : message(msg), context(ctx), location(loc), errorHasContext(true) {}

const string LanguageException::getMessage() const noexcept { return message; }

const string LanguageException::getContext() const noexcept { return context; }

unsigned LanguageException::getLocation() const noexcept { return location; }

bool LanguageException::hasContext() const noexcept { return errorHasContext; }

RuntimeError::RuntimeError(const string& msg) noexcept
    : LanguageException(msg) {}

const string RuntimeError::getKind() const noexcept { return "Runtime Error:"; }

StackOverflowError::StackOverflowError(unsigned long limit) noexcept
    : LanguageException("Stack has exceeded configured limit of " +
                        to_string(limit) + ".") {}

const string StackOverflowError::getKind() const noexcept {
  return "Stack overflowed:";
}

StackUnderflowError::StackUnderflowError() noexcept
    : LanguageException(
          "Stack is empty, but attempted to access element from stack.") {}

const string StackUnderflowError::getKind() const noexcept {
  return "Stack underflowed:";
}

StopError::StopError() noexcept
    : LanguageException("Ctrl-c (SIGINTR) sent, interpreter stopping.") {}

const string StopError::getKind() const noexcept { return "Manual interrupt:"; }

SyntaxError::SyntaxError(const string& msg) noexcept : LanguageException(msg) {}

SyntaxError::SyntaxError(const string& msg, const string& ctx,
                         size_t pos) noexcept
    : LanguageException(msg, ctx, pos) {}

const string SyntaxError::getKind() const { return "Syntax error:"; }

TypeError::TypeError(const StackElement& expected,
                     const StackElement& given) noexcept
    : LanguageException("Expected " + static_cast<string>(expected) +
                        "\nGiven " + static_cast<string>(given)) {}

TypeError::TypeError(const StackElement& expected) noexcept
    : LanguageException("Expected " + static_cast<string>(expected) +
                        " but reached the bottom of the stack instead.") {}

const string TypeError::getKind() const noexcept { return "Type Mismatch:"; }
}  // namespace exceptions
}  // namespace stacklang