#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_BOOLEANELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_BOOLEANELEMENT_H_

#include <string>

#include "language/stack/stackElement.h"

namespace stacklang {
namespace stackelements {
using std::string;

class BooleanElement : public StackElement {
 public:
  static const char* TSTR;
  static const char* FSTR;

  explicit BooleanElement(bool) noexcept;
  BooleanElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  bool getData() const noexcept;

 private:
  bool data;
};
}  // namespace stackelements
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_BOOLEANELEMENT_H