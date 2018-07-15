#ifndef STACKLANG_LANGUAGE_EXCEPTONS_ARGUMENTERROR_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_ARGUMENTERROR_H_

#include "language/exceptions/languageException.h"

namespace StackLang {
namespace Exceptions {
class ArgumentError : public LanguageException {
 public:
  ArgumentError(const string& msg);
  ArgumentError(const string& msg, const string& actual, size_t posDif);
  ArgumentError(const ArgumentError&) = default;

  ArgumentError& operator=(const ArgumentError&) = default;

  const string getKind() const override;
};
}  // namespace Exceptions
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_ARGUMENTERROR_H_