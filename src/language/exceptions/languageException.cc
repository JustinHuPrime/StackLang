#include "language/exceptions/languageException.h"

namespace StackLang {
namespace Exceptions {
LanguageException::LanguageException(const string& msg)
    : message(msg), errorHasContext(false) {}

LanguageException::LanguageException(const string& msg, const string& ctx,
                                     unsigned loc)
    : message(msg), context(ctx), location(loc), errorHasContext(true) {}

const string LanguageException::getMessage() const { return message; }

const string LanguageException::getContext() const { return context; }

unsigned LanguageException::getLocation() const { return location; }

bool LanguageException::hasContext() const { return errorHasContext; }
}  // namespace Exceptions
}  // namespace StackLang