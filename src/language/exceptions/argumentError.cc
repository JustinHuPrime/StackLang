#include "language/exceptions/argumentError.h"

namespace StackLang {
namespace Exceptions {
using std::to_string;

ArgumentError::ArgumentError(const string& message)
    : LanguageException(message) {}

ArgumentError::ArgumentError(const string& message, const string& actual,
                             size_t posDif)
    : LanguageException(message, actual, posDif) {}

const string ArgumentError::getKind() const {
  return "Command line arguments invalid:";
}
}  // namespace Exceptions
}  // namespace StackLang