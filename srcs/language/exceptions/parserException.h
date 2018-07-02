#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

#include "language/exceptions/languageException.h"

namespace StackLang
{
namespace Exceptions
{
class ParserException : public LanguageException
{
  public:
    ParserException (const string&, const string&, unsigned);

    const string getKind () const override;
};
} // namespace Exceptions
} // namespace StackLang

#endif /* PARSEREXCEPTION_H */