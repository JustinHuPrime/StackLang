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

LanguageException::LanguageException(const string& msg,
                                     list<CommandElement*> trace) noexcept
    : message(msg), errorHasContext(false), stacktrace(trace) {}

LanguageException::LanguageException(const string& msg, const string& ctx,
                                     size_t loc,
                                     list<CommandElement*> trace) noexcept
    : message(msg),
      context(ctx),
      location(loc),
      errorHasContext(true),
      stacktrace(trace) {}

const string& LanguageException::getMessage() const noexcept { return message; }
const string& LanguageException::getContext() const noexcept { return context; }
size_t LanguageException::getLocation() const noexcept { return location; }
bool LanguageException::hasContext() const noexcept { return errorHasContext; }
const list<CommandElement*>& LanguageException::getTrace() const noexcept {
  return stacktrace;
}

RuntimeError::RuntimeError(const string& msg,
                           list<CommandElement*> trace) noexcept
    : LanguageException(msg, trace) {}

string RuntimeError::getKind() const noexcept { return "Runtime Error:"; }

StackOverflowError::StackOverflowError(size_t limit,
                                       list<CommandElement*> trace) noexcept
    : LanguageException(
          "Stack has exceeded configured limit of " + to_string(limit) + ".",
          trace) {}

string StackOverflowError::getKind() const noexcept {
  return "Stack overflowed:";
}

StackUnderflowError::StackUnderflowError(list<CommandElement*> trace) noexcept
    : LanguageException(
          "Stack is empty, but attempted to access element from stack.",
          trace) {}

string StackUnderflowError::getKind() const noexcept {
  return "Stack underflowed:";
}

StopError::StopError(list<CommandElement*> trace) noexcept
    : LanguageException("Ctrl-c (SIGINTR) sent, interpreter stopping.", trace) {
}

string StopError::getKind() const noexcept { return "Manual interrupt:"; }

SyntaxError::SyntaxError(const string& msg,
                         list<CommandElement*> trace) noexcept
    : LanguageException(msg, trace) {}

SyntaxError::SyntaxError(const string& msg, const string& ctx, size_t pos,
                         list<CommandElement*> trace) noexcept
    : LanguageException(msg, ctx, pos, trace) {}

string SyntaxError::getKind() const noexcept { return "Syntax error:"; }

TypeError::TypeError(const StackElement& expected, const StackElement& given,
                     list<CommandElement*> trace) noexcept
    : LanguageException("Expected " + static_cast<string>(expected) +
                            "\nGiven " + static_cast<string>(given),
                        trace) {}

TypeError::TypeError(const StackElement& expected,
                     list<CommandElement*> trace) noexcept
    : LanguageException("Expected " + static_cast<string>(expected) +
                            " but reached the bottom of the stack instead.",
                        trace) {}

string TypeError::getKind() const noexcept { return "Type Mismatch:"; }
}  // namespace exceptions
}  // namespace stacklang