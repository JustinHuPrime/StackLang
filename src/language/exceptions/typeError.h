#ifndef STACKLANG_LANGUAGE_EXCEPTONS_TYPEERROR_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_TYPEERROR_H_

#include "language/exceptions/languageException.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/typeElement.h"

namespace StackLang {
namespace Exceptions {
using StackLang::StackElements::TypeElement;
class TypeError : public LanguageException {
 public:
  TypeError(const TypeElement, const StackElement*);
  TypeError(const TypeElement);
  TypeError(const TypeError&) = default;

  TypeError& operator=(const TypeError&) = default;

  const string getKind() const override;
};
}  // namespace Exceptions
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_TYPEERROR_H_