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

        private:

        bool data;
    };

    namespace BooleanConstants
    {
        const string TSTR = "true";
        const string FSTR = "false";
    }
}

#endif /* BOOLEANELEMENT_H */