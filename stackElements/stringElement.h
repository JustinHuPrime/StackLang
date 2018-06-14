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

        StringElement (string);

        operator const string () const override;
        const string& getData () const;

        private:

        string data;
    };
}

#endif /* STRINGELEMENT_H */