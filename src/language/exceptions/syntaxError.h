#ifndef STACKLANG_LANGUAGE_EXCEPTONS_SYNTAXERROR_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_SYNTAXERROR_H_

#include "language/exceptions/languageException.h"

namespace stacklang {
namespace exceptions {
class SyntaxError : public LanguageException {
 public:
  SyntaxError(const string& msg);
  SyntaxError(const string& msg, const string& ctx, size_t pos);
  SyntaxError(const SyntaxError&) = default;

  SyntaxError& operator=(const SyntaxError&) = default;

  const string getKind() const override;
};
}  // namespace exceptions
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_SYNTAXERROR_H_