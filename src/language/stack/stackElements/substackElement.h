#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_SUBSTACKELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_SUBSTACKELEMENT_H_

#include <string>

#include "language/stack.h"
#include "language/stack/stackElement.h"

namespace stacklang {
namespace stackelements {
using std::string;

class SubstackElement : public StackElement {
 public:
  static SubstackElement* parse(const string&);
  explicit SubstackElement(const Stack&) noexcept;
  SubstackElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  const Stack& getData() const noexcept;

  static const char* SUBSTACK_BEGIN;
  static const char* SUBSTACK_END;

 private:
  static const char* SUBSTACK_SEPARATOR;
  static const char* SUBSTACK_EMPTY;

  Stack data;
};
}  // namespace stackelements
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_SUBSTACKELEMENT_H_