#ifndef BOOLEANELEMENT_H
#define BOOLEANELEMENT_H

#include "stackElement.h"
#include <string>
using std::string;

namespace StackElements
{
    class BooleanElement : public StackElement
    {
        public:

        BooleanElement (bool);

        operator const char* () const override;

        static const string TSTR;
        static const string FSTR;

        private:

        bool data;
    };
}

#endif /* BOOLEANELEMENT_H */