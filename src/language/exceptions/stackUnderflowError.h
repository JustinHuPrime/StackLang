#ifndef STACKLANG_LANGUAGE_EXCEPTONS_STACKUNDERFLOWERROR_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_STACKUNDERFLOWERROR_H_

#include "language/exceptions/languageException.h"

namespace StackLang {
namespace Exceptions {
class StackUnderflowError : public LanguageException {
 public:
  StackUnderflowError();
  StackUnderflowError(const StackUnderflowError&) = default;

  StackUnderflowError& operator=(const StackUnderflowError&) = default;

  const string getKind() const override;
};
}  // namespace Exceptions
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_STACKUNDERFLOWERROR_H_