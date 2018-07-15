#include "language/exceptions/syntaxError.h"

namespace StackLang {
namespace Exceptions {

SyntaxError::SyntaxError(const string& msg) : LanguageException(msg) {}

SyntaxError::SyntaxError(const string& msg, const string& ctx, size_t pos)
    : LanguageException(msg, ctx, pos) {}

const string SyntaxError::getKind() const { return "Syntax error:"; }
}  // namespace Exceptions
}  // namespace StackLang