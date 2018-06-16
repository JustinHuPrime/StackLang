#ifndef PARSERERROR_H
#define PARSERERROR_H

#include "language/exceptions/languageError.h"

namespace StackLang
{
    namespace Exceptions
    {
        class ParserError : public LanguageError
        {
            ParserError (const string&, const string&, int);

            const string getKind () const override;
        };
    }
}

#endif /* PARSERERROR_H */