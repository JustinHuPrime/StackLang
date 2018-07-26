#ifndef STACKLANG_LANGUAGE_EXCEPTONS_PARSEREXCEPTION_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_PARSEREXCEPTION_H_

#include "language/exceptions/languageException.h"

namespace stacklang {
namespace exceptions {
class ParserException : public LanguageException {
 public:
  ParserException(const string&, const string&, unsigned);
  ParserException(const ParserException&) = default;

  ParserException& operator=(const ParserException&) = default;

  const string getKind() const override;
};
}  // namespace exceptions
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_PARSEREXCEPTION_H_