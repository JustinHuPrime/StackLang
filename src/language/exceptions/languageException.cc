// Copyright 2018 Justin Hu
//
// This file is part of the StackLang interpreter.
//
// The StackLang interpreter is free software: you can redistribute it and / or
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

// Implenetation of language exceptions. See other directories for specific
// implenetations. Other implenentations mostly limited to a constructor and a
// getKind method.

#include "language/exceptions/languageException.h"

namespace stacklang {
namespace exceptions {
LanguageException::LanguageException(const string& msg)
    : message(msg), errorHasContext(false) {}

LanguageException::LanguageException(const string& msg, const string& ctx,
                                     unsigned loc)
    : message(msg), context(ctx), location(loc), errorHasContext(true) {}

const string LanguageException::getMessage() const { return message; }

const string LanguageException::getContext() const { return context; }

unsigned LanguageException::getLocation() const { return location; }

bool LanguageException::hasContext() const { return errorHasContext; }
}  // namespace exceptions
}  // namespace stacklang