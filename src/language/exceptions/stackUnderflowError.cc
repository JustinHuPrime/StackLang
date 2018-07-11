#include "language/exceptions/stackUnderflowError.h"

namespace StackLang {
namespace Exceptions {
using std::to_string;

StackUnderflowError::StackUnderflowError()
    : LanguageException(
          "Stack is empty, but attempted to access element from stack.") {}

const string StackUnderflowError::getKind() const {
  return "Stack underflowed:";
}
}  // namespace Exceptions
}  // namespace StackLang