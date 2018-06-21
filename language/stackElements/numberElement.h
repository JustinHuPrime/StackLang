#ifndef NUMBERELEMENT_H
#define NUMBERELEMENT_H

#include "language/stackElement.h"
#include "utils/bigDecimal.h"
#include <string>
using std::string;

namespace StackLang
{
    namespace StackElements
    {
        class NumberElement : public StackElement
        {
            public:

            NumberElement (BigDecimal);

            operator const string () const override;
            BigDecimal getData ();

            private:

            BigDecimal data;
        };
    }

    namespace CharSet
    {
        const string ALLOWED_NUMBER = "1234567890.'";
    }
}

#endif /* NUMBERELEMENT_H */