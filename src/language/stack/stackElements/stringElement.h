#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_STRINGELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_STRINGELEMENT_H_

#include <string>

#include "language/stack/stackElement.h"

namespace stacklang {
namespace stackelements {
using std::string;

class StringElement : public StackElement {
 public:
  static StringElement* parse(const string&);
  explicit StringElement(string) noexcept;
  StringElement* clone() const noexcept override;

  explicit operator string() const noexcept override;
  const string& getData() const noexcept;

  static const char QUOTE_CHAR;

 private:
  string data;
};
}  // namespace stackelements
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_STRINGELEMENT_H_