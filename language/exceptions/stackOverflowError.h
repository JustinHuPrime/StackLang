#ifndef PARSERERROR_H
#define PARSERERROR_H

#include "language/exceptions/languageError.h"

namespace StackLang
{
namespace Exceptions
{
class StackOverflowError : public LanguageError
{
  public:
    StackOverflowError (unsigned long);

    const string getKind () const override;
};
} // namespace Exceptions
} // namespace StackLang

#endif /* PARSERERROR_H */