#include "language/exceptions/argumentError.h"

namespace StackLang {
namespace Exceptions {
using std::to_string;

ArgumentError::ArgumentError(const string& msg) : LanguageException(msg) {}

ArgumentError::ArgumentError(const string& msg, const string& actual,
                             size_t posDif)
    : LanguageException(msg, actual, posDif) {}

const string ArgumentError::getKind() const {
  return "Command line arguments invalid:";
}
}  // namespace Exceptions
}  // namespace StackLang