#include "language/stack/stackElements/typeElement.h"

#include <array>

namespace StackLang {
namespace StackElements {
using StackLang::StackElement;
using std::array;

TypeElement::TypeElement(DataType type)
    : StackElement(StackElement::DataType::Type), data(type) {}

TypeElement* TypeElement::clone() const { return new TypeElement(data); }

TypeElement::operator const string() const { return to_string(data); }

StackElement::DataType TypeElement::getData() const { return data; }

string TypeElement::to_string(StackElement::DataType type) {
  return TYPES()[static_cast<unsigned>(type)];
}

const vector<string>& TypeElement::TYPES() {
  static vector<string>* TYPES = new vector<string>(
      {"Number", "String", "Boolean", "Substack", "Type", "Command", "Any"});
  return *TYPES;
}
}  // namespace StackElements
}  // namespace StackLang