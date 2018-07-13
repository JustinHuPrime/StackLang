#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_STRINGELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_STRINGELEMENT_H_

#include <string>

#include "language/stack/stackElement.h"

namespace StackLang {
namespace StackElements {
using std::string;

class StringElement : public StackElement {
 public:
  static StringElement* parse(const string&);
  explicit StringElement(string);
  StringElement* clone() const override;

  explicit operator const string() const override;
  string& getData();

 private:
  string data;
};
}  // namespace StackElements
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_STRINGELEMENT_H_