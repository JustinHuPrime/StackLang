#include "language/exceptions/stackOverflowError.h"

namespace StackLang {
namespace Exceptions {
using std::to_string;

StackOverflowError::StackOverflowError(unsigned long limit)
    : LanguageException("Stack has exceeded configured limit of " +
                        to_string(limit) + ".") {}

const string StackOverflowError::getKind() const { return "Stack overflowed:"; }
}  // namespace Exceptions
}  // namespace StackLang