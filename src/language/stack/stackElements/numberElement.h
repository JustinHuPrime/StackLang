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

  static NumberElement* parse(const string&);

  explicit NumberElement(string);
  NumberElement* clone() const override;

  explicit operator const string() const override;
  mpq_class& getData();

 private:
  mpq_class data;
};
}  // namespace StackElements
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_NUMBERELEMENT_H_