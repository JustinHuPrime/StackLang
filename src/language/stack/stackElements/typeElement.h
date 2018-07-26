#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_

#include <string>
#include <vector>

#include "language/stack/stackElement.h"

namespace stacklang {
namespace stackelements {
using std::string;
using std::vector;
class TypeElement : public StackElement {
 public:
  static TypeElement* parse(const string&);
  explicit TypeElement(DataType) noexcept;
  TypeElement(DataType, TypeElement*) noexcept;
  TypeElement(const TypeElement&) noexcept;
  TypeElement& operator=(const TypeElement&) noexcept;
  TypeElement(TypeElement&&) noexcept;
  TypeElement& operator=(TypeElement&&) noexcept;
  TypeElement* clone() const noexcept override;
  ~TypeElement() noexcept;

  explicit operator string() const noexcept override;
  DataType getData() const noexcept;
  const TypeElement* getSpecialization() const noexcept;

  static string to_string(DataType) noexcept;

  static const vector<string>& TYPES() noexcept;

  static const char* PARENS;

 private:
  DataType data;
  TypeElement* specialization;
};
}  // namespace stackelements
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_TYPEELEMENT_H_