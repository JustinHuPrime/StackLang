#ifndef STACKLANG_LANGUAGE_EXCEPTONS_TYPEERROR_H_
#define STACKLANG_LANGUAGE_EXCEPTONS_TYPEERROR_H_

#include "language/exceptions/languageException.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/typeElement.h"

namespace StackLang {
namespace Exceptions {
class TypeError : public LanguageException {
 public:
  TypeError(StackElement::DataType, StackElement*);
  TypeError(const TypeError&) = default;

  TypeError& operator=(const TypeError&) = default;

  const string getKind() const override;
};
}  // namespace Exceptions
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_EXCEPTONS_TYPEERROR_H_