#include "language/exceptions/typeError.h"

namespace StackLang {
namespace Exceptions {
using StackLang::StackElements::TypeElement;

TypeError::TypeError(const StackElement::DataType expected,
                     const StackElement* given)
    : LanguageException("Expected " + TypeElement::to_string(expected) +
                        "\nGiven " + string(*given)) {}

TypeError::TypeError(const StackElement::DataType expected)
    : LanguageException("Expected " + TypeElement::to_string(expected) +
                        " but reached the bottom of the stack instead.") {}

const string TypeError::getKind() const { return "Type Mismatch:"; }
}  // namespace Exceptions
}  // namespace StackLang