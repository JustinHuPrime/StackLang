#include "language/exceptions/stopError.h"

namespace stacklang {
namespace exceptions {
StopError::StopError()
    : LanguageException("Ctrl-c (SIGINTR) sent, interpreter stopping.") {}

const string StopError::getKind() const { return "Manual interrupt:"; }
}  // namespace exceptions
}  // namespace stacklang