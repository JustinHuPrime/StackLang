#ifndef STRINGELEMENT_H
#define STRINGELEMENT_H

#include "stackElement.h"
#include <string>
using std::string;

namespace StackElements
{
    class StringElement : public StackElement
    {
        public:

        StringElement ();

        operator const char* () const override;

        private:

        string data;
    };
}

#endif /* STRINGELEMENT_H */