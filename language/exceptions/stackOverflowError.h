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
            
            StackOverflowError (int);

            const string getKind () const override;
        };
    }
}

#endif /* PARSERERROR_H */