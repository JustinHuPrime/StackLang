#include "language/exceptions/stackUnderflowError.h"

namespace stacklang {
namespace exceptions {
using std::to_string;

StackUnderflowError::StackUnderflowError()
    : LanguageException(
          "Stack is empty, but attempted to access element from stack.") {}

const string StackUnderflowError::getKind() const {
  return "Stack underflowed:";
}
}  // namespace exceptions
}  // namespace stacklang