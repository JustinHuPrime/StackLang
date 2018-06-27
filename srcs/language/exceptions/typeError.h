#ifndef TYPEERROR_H
#define TYPEERROR_H

#include "language/exceptions/languageError.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/typeElement.h"

namespace StackLang
{
namespace Exceptions
{
class TypeError : public LanguageError
{
  public:
    TypeError (StackElement::DataType, StackElement*);

    const string getKind () const override;
};
} // namespace Exceptions
} // namespace StackLang

#endif /* TYPEERROR_H */