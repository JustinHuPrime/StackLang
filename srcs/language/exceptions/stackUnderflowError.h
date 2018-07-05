#ifndef STACKUNDERFLOWERROR_H
#define STACKUNDERFLOWERROR_H

#include "language/exceptions/languageException.h"

namespace StackLang
{
namespace Exceptions
{
class StackUnderflowError : public LanguageException
{
  public:
    StackUnderflowError();

    const string getKind() const override;
};
} // namespace Exceptions
} // namespace StackLang

#endif /* STACKUNDERFLOWERROR_H */