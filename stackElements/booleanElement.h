#ifndef BOOLEANELEMENT_H
#define BOOLEANELEMENT_H

#include "stackElement.h"

namespace StackElements
{
    class BooleanElement : public StackElement
    {
        public:

        BooleanElement ();

        operator const char* () const override;

        private:

        bool data;
    };
}

#endif /* BOOLEANELEMENT_H */