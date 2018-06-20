#ifndef TYPEERROR_H
#define TYPEERROR_H

#include "language/stackElement.h"
#include "language/stackElements/typeElement.h"
#include "language/exceptions/languageError.h"

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
    }
}

#endif /* TYPEERROR_H */