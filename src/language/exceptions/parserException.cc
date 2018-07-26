#include "language/exceptions/parserException.h"

namespace stacklang {
namespace exceptions {
ParserException::ParserException(const string& msg, const string& ctx,
                                 unsigned loc)
    : LanguageException(msg, ctx, loc) {}

const string ParserException::getKind() const { return "Could not parse:"; }
}  // namespace exceptions
}  // namespace stacklang