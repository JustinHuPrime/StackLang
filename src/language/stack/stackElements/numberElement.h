#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_NUMBERELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_NUMBERELEMENT_H_

#include <string>

#include <gmp.h>
#include <gmpxx.h>

#include "language/stack/stackElement.h"

namespace StackLang {
namespace StackElements {
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

  explicit operator const string() const noexcept override;
  const mpq_class& getData() const noexcept;
  bool isExact() const noexcept;

 private:
  mpq_class data;
  bool exact;
};
}  // namespace StackElements
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_NUMBERELEMENT_H_