#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_

#include "language/stack/stackElement.h"

#include <string>
#include <vector>

namespace StackLang {
namespace StackElements {
using std::string;
using std::vector;
class TypeElement : public StackElement {
 public:
  explicit TypeElement(DataType);
  TypeElement* clone() const override;

  explicit operator const string() const override;
  DataType& getData();

  static string to_string(DataType);

  static const vector<string>& TYPES();

 private:
  DataType data;
};
}  // namespace StackElements
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_