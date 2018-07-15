#include "language/exceptions/stopError.h"

namespace StackLang {
namespace Exceptions {

StopError::StopError()
    : LanguageException("Ctrl-c (SIGINTR) sent, interpreter stopping.") {}

const string StopError::getKind() const { return "Manual interrupt:"; }
}  // namespace Exceptions
}  // namespace StackLang