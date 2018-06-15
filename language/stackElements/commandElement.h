#ifndef COMMANDELEMENT_H
#define COMMANDELEMENT_H

#include "language/stackElement.h"
#include <string>
using std::string;

namespace StackLang
{
    namespace StackElements
    {
        class CommandElement : public StackElement
        {
            public:

            CommandElement (string);

            operator const string () const override;
            string& getData ();

            private:

            string data;
        };
    }
}

#endif /* COMMANDELEMENT_H */