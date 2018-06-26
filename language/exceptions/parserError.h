#ifndef PARSERERROR_H
#define PARSERERROR_H

#include "language/exceptions/languageError.h"

namespace StackLang
{
    namespace Exceptions
    {
        class ParserError : public LanguageError
        {
            public:
            
            ParserError (const string&, const string&, unsigned);

            const string getKind () const override;
        };
    }
}

#endif /* PARSERERROR_H */