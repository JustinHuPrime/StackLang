#include "language/exceptions/syntaxError.h"

namespace stacklang {
namespace exceptions {
SyntaxError::SyntaxError(const string& msg) : LanguageException(msg) {}

SyntaxError::SyntaxError(const string& msg, const string& ctx, size_t pos)
    : LanguageException(msg, ctx, pos) {}

const string SyntaxError::getKind() const { return "Syntax error:"; }
}  // namespace exceptions
}  // namespace stacklang