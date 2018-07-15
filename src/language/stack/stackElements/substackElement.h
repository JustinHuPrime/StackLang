#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_SUBSTACKELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_SUBSTACKELEMENT_H_

#include <string>

#include "language/stack.h"
#include "language/stack/stackElement.h"

namespace StackLang {
namespace StackElements {
using std::string;

class SubstackElement : public StackElement {
 public:
  static SubstackElement* parse(const string&);
  explicit SubstackElement(const Stack&);
  SubstackElement* clone() const override;

  explicit operator const string() const override;
  const Stack& getData() const;

 private:
  static const char* SUBSTACK_BEGIN;
  static const char* SUBSTACK_END;
  static const char* SUBSTACK_SEPARATOR;
  static const char* SUBSTACK_EMPTY;

  Stack data;
};
}  // namespace StackElements
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_SUBSTACKELEMENT_H_