#ifndef STACKLANG_LANGUAGE_EXCEPTONS_STACKOVERFLOWERROR_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_STACKOVERFLOWERROR_H_

#include "language/exceptions/languageException.h"

namespace stacklang {
namespace exceptions {
class StackOverflowError : public LanguageException {
 public:
  explicit StackOverflowError(unsigned long);
  StackOverflowError(const StackOverflowError&) = default;

  StackOverflowError& operator=(const StackOverflowError&) = default;

  const string getKind() const override;
};
}  // namespace exceptions
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_STACKOVERFLOWERROR_H_