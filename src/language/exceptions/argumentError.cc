#include "language/exceptions/argumentError.h"

namespace stacklang {
namespace exceptions {
using std::to_string;

ArgumentError::ArgumentError(const string& msg) : LanguageException(msg) {}

ArgumentError::ArgumentError(const string& msg, const string& actual,
                             size_t posDif)
    : LanguageException(msg, actual, posDif) {}

const string ArgumentError::getKind() const {
  return "Command line arguments invalid:";
}
}  // namespace exceptions
}  // namespace stacklang