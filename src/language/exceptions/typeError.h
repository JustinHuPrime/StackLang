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

#ifndef STACKLANG_LANGUAGE_EXCEPTONS_TYPEERROR_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_TYPEERROR_H_

#include "language/exceptions/languageException.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/typeElement.h"

namespace stacklang {
namespace exceptions {
using stacklang::stackelements::TypeElement;
class TypeError : public LanguageException {
 public:
  TypeError(const TypeElement, const StackElement*);
  TypeError(const TypeElement);
  TypeError(const TypeError&) = default;

  TypeError& operator=(const TypeError&) = default;

  const string getKind() const override;
};
}  // namespace exceptions
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_TYPEERROR_H_