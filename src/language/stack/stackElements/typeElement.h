#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_

#include <string>
#include <vector>

#include "language/stack/stackElement.h"

namespace StackLang {
namespace StackElements {
using std::string;
using std::vector;
class TypeElement : public StackElement {
 public:
  static TypeElement* parse(const string&);
  explicit TypeElement(DataType);
  TypeElement(DataType, TypeElement*);
  TypeElement(const TypeElement&);
  TypeElement& operator=(const TypeElement&);
  TypeElement(TypeElement&&);
  TypeElement& operator=(TypeElement&&);
  TypeElement* clone() const override;
  ~TypeElement();

  explicit operator const string() const override;
  DataType getData() const;

  static string to_string(DataType);

  static const vector<string>& TYPES();

  static const char* PARENS;

 private:
  DataType data;
  TypeElement* specialization;
};
}  // namespace StackElements
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_