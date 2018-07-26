#ifndef STACKLANG_LANGUAGE_EXCEPTONS_STOPERROR_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_STOPERROR_H_

#include "language/exceptions/languageException.h"

namespace stacklang {
namespace exceptions {
class StopError : public LanguageException {
 public:
  StopError();
  StopError(const StopError&) = default;

  StopError& operator=(const StopError&) = default;

  const string getKind() const override;
};
}  // namespace exceptions
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_STOPERROR_H_