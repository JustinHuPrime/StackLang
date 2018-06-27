#ifndef NUMBERELEMENT_H
#define NUMBERELEMENT_H

#include "language/stack/stackElement.h"
#include <string>
using std::string;

namespace StackLang
{
    namespace StackElements
    {
        class NumberElement : public StackElement
        {
            public:

            NumberElement (double);

            operator const string () const override;
            double getData ();

            private:

            double data;
        };
    }

    namespace CharSet
    {
        const string ALLOWED_NUMBER = "1234567890.'";
    }
}

#endif /* NUMBERELEMENT_H */