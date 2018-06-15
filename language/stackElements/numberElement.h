#ifndef NUMBERELEMENT_H
#define NUMBERELEMENT_H

#include "language/stackElement.h"
#include <string>
using std::string;
using std::to_string;

namespace StackLang
{
    namespace StackElements
    {
        class NumberElement : public StackElement
        {
            public:

            NumberElement (double);

            operator const string () const override;
            double& getData ();

            private:

            double data;
        };
    }
}

#endif /* NUMBERELEMENT_H */