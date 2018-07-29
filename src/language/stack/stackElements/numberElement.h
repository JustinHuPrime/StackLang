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

#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_NUMBERELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_NUMBERELEMENT_H_

#include <string>

#include <gmp.h>
#include <gmpxx.h>

#include "language/stack/stackElement.h"

namespace stacklang {
namespace stackelements {
using std::string;

class NumberElement : public StackElement {
 public:
  static const char* ALLOWED_NUMBER;
  static const char* NUMBER_SIGNS;
  static const char INEXACT_SIGNAL;

  static NumberElement* parse(const string&);

  explicit NumberElement(string, bool isExact = true) noexcept;
  explicit NumberElement(const mpq_class&, bool isExact = true) noexcept;
  NumberElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  const mpq_class& getData() const noexcept;
  bool isExact() const noexcept;

 private:
  mpq_class data;
  bool exact;
};
}  // namespace stackelements
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_NUMBERELEMENT_H_