#include "language/exceptions/typeError.h"

namespace stacklang {
namespace exceptions {

TypeError::TypeError(const TypeElement expected, const StackElement* given)
    : LanguageException("Expected " + static_cast<string>(expected) +
                        "\nGiven " + static_cast<string>(*given)) {}

TypeError::TypeError(const TypeElement expected)
    : LanguageException("Expected " + static_cast<string>(expected) +
                        " but reached the bottom of the stack instead.") {}

const string TypeError::getKind() const { return "Type Mismatch:"; }
}  // namespace exceptions
}  // namespace stacklang