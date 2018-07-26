#include "language/stack/stackElements/booleanElement.h"

namespace StackLang {
namespace StackElements {
const char* BooleanElement::TSTR = "true";
const char* BooleanElement::FSTR = "false";

BooleanElement::BooleanElement(bool b) noexcept
    : StackElement(StackElement::DataType::Boolean), data(b) {}

BooleanElement* BooleanElement::clone() const noexcept {
  return new BooleanElement(data);
}

BooleanElement::operator string() const noexcept { return data ? TSTR : FSTR; }

bool BooleanElement::getData() const noexcept { return data; }
}  // namespace StackElements
}  // namespace StackLang