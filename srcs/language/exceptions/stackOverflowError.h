#ifndef STACKOVERFLOWERROR_H
#define STACKOVERFLOWERROR_H

#include "language/exceptions/languageException.h"

namespace StackLang
{
namespace Exceptions
{
class StackOverflowError : public LanguageException
{
  public:
    StackOverflowError (unsigned long);

    const string getKind () const override;
};
} // namespace Exceptions
} // namespace StackLang

#endif /* STACKOVERFLOWERROR_H */