#ifndef BOOLEANELEMENT_H
#define BOOLEANELEMENT_H

#include "language/stackElement.h"
#include <string>
using std::string;

namespace StackLang
{
    namespace StackElements
    {
        class BooleanElement : public StackElement
        {
            public:

            BooleanElement (bool);

            operator const string () const override;
            bool& getData ();

            private:

            bool data;
        };

        namespace BooleanConstants
        {
            const string TSTR = "true";
            const string FSTR = "false";
        }
    }
}

#endif /* BOOLEANELEMENT_H */