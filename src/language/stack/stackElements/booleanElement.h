#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_BOOLEANELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_BOOLEANELEMENT_H

#include <string>

#include "language/stack/stackElement.h"

namespace StackLang {
namespace StackElements {
using std::string;

class BooleanElement : public StackElement {
 public:
  static const char* TSTR;
  static const char* FSTR;

  explicit BooleanElement(bool);
  BooleanElement* clone() const override;

  explicit operator const string() const override;
  bool& getData();

 private:
  bool data;
};
}  // namespace StackElements
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_BOOLEANELEMENT_H