#ifndef NUMBERELEMENT_H
#define NUMBERELEMENT_H

#include "stackElement.h"

namespace StackElements
{
    class NumberElement : public StackElement
    {
        public:

        NumberElement (double);

        operator const string () const override;

        private:

        double data;
    };
}

#endif /* NUMBERELEMENT_H */