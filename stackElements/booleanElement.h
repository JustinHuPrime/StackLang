#ifndef BOOLEANELEMENT_H
#define BOOLEANELEMENT_H

#include "stackElement.h"

namespace StackElements
{
    class BooleanElement : public StackElement
    {
        public:

        BooleanElement (bool);

        operator const char* () const override;

        private:

        bool data;
    };

    namespace BooleanConstants
    {
        const string TCSTR;
        const string FCSTR;
    }
}

#endif /* BOOLEANELEMENT_H */